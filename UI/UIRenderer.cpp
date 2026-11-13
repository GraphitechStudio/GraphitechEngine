/**
 * @file UIRenderer.cpp
 * @brief Implementation of OpenGL UI renderer
 */

#include "UIRenderer.hpp"
#include "UIContext.hpp"
#include "BitmapFont.hpp"
#include <iostream>
#include <cmath>
#include <vector>
#include <algorithm>

namespace Graphitech {
namespace UI {

// Simple vertex shader for UI elements
static const char* vertexShaderSource = R"(
#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoord;

out vec4 vertexColor;
out vec2 TexCoord;

uniform mat4 projection;

void main() {
    gl_Position = projection * vec4(aPos, 0.0, 1.0);
    vertexColor = aColor;
    TexCoord = aTexCoord;
}
)";

// Fragment shader for UI elements with optional texture sampling
static const char* fragmentShaderSource = R"(
#version 330 core
in vec4 vertexColor;
in vec2 TexCoord;
out vec4 FragColor;

uniform sampler2D uTexture;
uniform bool useTexture;

void main() {
    if (useTexture) {
        vec4 tex = texture(uTexture, TexCoord);
        FragColor = tex * vertexColor;
    } else {
        FragColor = vertexColor;
    }
}
)";

UIRenderer::UIRenderer() 
    : shaderProgram_(0), vao_(0), vbo_(0), ebo_(0),
      windowWidth_(0), windowHeight_(0) {}

UIRenderer::~UIRenderer() {
    Shutdown();
}

bool UIRenderer::Initialize(int windowWidth, int windowHeight) {
    windowWidth_ = windowWidth;
    windowHeight_ = windowHeight;
    
    CreateShaders();
    CreateBuffers();
    
    // Set up orthographic projection
    projection_ = glm::ortho(0.0f, (float)windowWidth_, (float)windowHeight_, 0.0f, -1.0f, 1.0f);
    
    // Enable blending for transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    
    return true;
}

void UIRenderer::Shutdown() {
    if (vao_) glDeleteVertexArrays(1, &vao_);
    if (vbo_) glDeleteBuffers(1, &vbo_);
    if (ebo_) glDeleteBuffers(1, &ebo_);
    if (shaderProgram_) glDeleteProgram(shaderProgram_);
}

void UIRenderer::CreateShaders() {
    // Compile vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, nullptr);
    glCompileShader(vertexShader);
    
    GLint success;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "Vertex shader compilation failed: " << infoLog << std::endl;
    }
    
    // Compile fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, nullptr);
    glCompileShader(fragmentShader);
    
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "Fragment shader compilation failed: " << infoLog << std::endl;
    }
    
    // Link shader program
    shaderProgram_ = glCreateProgram();
    glAttachShader(shaderProgram_, vertexShader);
    glAttachShader(shaderProgram_, fragmentShader);
    glLinkProgram(shaderProgram_);
    
    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &success);
    if (!success) {
        char infoLog[512];
        glGetProgramInfoLog(shaderProgram_, 512, nullptr, infoLog);
        std::cerr << "Shader program linking failed: " << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void UIRenderer::CreateBuffers() {
    glGenVertexArrays(1, &vao_);
    glGenBuffers(1, &vbo_);
    glGenBuffers(1, &ebo_);
    
    glBindVertexArray(vao_);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    // Now each vertex has: pos(2) + color(4) + texcoord(2) = 8 floats
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 8 * 1000, nullptr, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * 1000, nullptr, GL_DYNAMIC_DRAW);

    // Position attribute (location 0)
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Color attribute (location 1)
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    // TexCoord attribute (location 2)
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glBindVertexArray(0);
}

void UIRenderer::SetWindowSize(int width, int height) {
    windowWidth_ = width;
    windowHeight_ = height;
    projection_ = glm::ortho(0.0f, (float)windowWidth_, (float)windowHeight_, 0.0f, -1.0f, 1.0f);
}

void UIRenderer::BeginFrame() {
    glUseProgram(shaderProgram_);
    GLint projLoc = glGetUniformLocation(shaderProgram_, "projection");
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, &projection_[0][0]);
    
    glBindVertexArray(vao_);
}

void UIRenderer::EndFrame() {
    // Draw tooltip overlay if any
    if (UI::UIContext::HasTooltip()) {
        std::string txt = UI::UIContext::GetTooltipText();
        glm::vec2 pos = UI::UIContext::GetTooltipPos();
        float fontSize = 14.0f;
        float scale = fontSize / (float)BitmapFont::FONT_CHAR_HEIGHT;
        float textW = (float)BitmapFont::GetTextWidth(txt, scale);
        float padding = 8.0f;
        float w = textW + padding * 2.0f;
        float h = fontSize + padding;
        float x = pos.x + 12.0f; // offset slightly from cursor
        float y = pos.y + 20.0f;
        if (x + w > (float)windowWidth_) x = (float)windowWidth_ - w - 4.0f;
        if (y + h > (float)windowHeight_) y = (float)windowHeight_ - h - 4.0f;

        // Background (semi-transparent dark)
        DrawRoundedRect(x, y, w, h, 6.0f, Color(0.08f, 0.08f, 0.08f, 0.92f), true);
        // Text
        DrawText(txt, x + padding, y + padding / 2.0f, fontSize, Color::White());
    }

    glBindVertexArray(0);
    glUseProgram(0);
}

void UIRenderer::DrawRect(float x, float y, float width, float height, const Color& color, bool filled) {
    // Vertex layout: x, y, r, g, b, a, u, v
    float vertices[] = {
        x,         y,          color.r, color.g, color.b, color.a, 0.0f, 0.0f,
        x + width, y,          color.r, color.g, color.b, color.a, 1.0f, 0.0f,
        x + width, y + height, color.r, color.g, color.b, color.a, 1.0f, 1.0f,
        x,         y + height, color.r, color.g, color.b, color.a, 0.0f, 1.0f,
    };

    // Filled: two triangles. Outline: the four corners in order, otherwise the
    // line loop would follow the triangle indices and draw a diagonal.
    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };
    const unsigned int outlineIndices[] = { 0, 1, 2, 3 };

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    if (filled) {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);
    } else {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(outlineIndices), outlineIndices, GL_STREAM_DRAW);
    }

    // Disable texture sampling for plain rects
    GLint useTexLoc = glGetUniformLocation(shaderProgram_, "useTexture");
    glUniform1i(useTexLoc, 0);

    if (filled) {
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    } else {
        glDrawElements(GL_LINE_LOOP, 4, GL_UNSIGNED_INT, 0);
    }
}

void UIRenderer::DrawRoundedRect(float x, float y, float width, float height, float /*radius*/, const Color& color, bool filled) {
    // For simplicity, draw a regular rect for now
    // TODO: Implement proper rounded corners with more vertices
    DrawRect(x, y, width, height, color, filled);
}

void UIRenderer::DrawCircle(float x, float y, float radius, const Color& color, bool filled) {
    // Choose segments based on circumference so larger circles get more segments (smoother)
    const float circumference = 2.0f * 3.14159265359f * radius;
    // target ~ one segment every 4 pixels on circumference
    int segments = std::max(12, std::min(256, (int)std::ceil(circumference / 4.0f)));

    // We need center + (segments + 1) perimeter + one duplicate for closure = segments + 2 vertices
    // Vertex layout expected by VAO: x,y, r,g,b,a, u,v (8 floats)
    std::vector<float> vertices((segments + 2) * 8, 0.0f);

    // Center vertex
    vertices[0] = x;
    vertices[1] = y;
    vertices[2] = color.r;
    vertices[3] = color.g;
    vertices[4] = color.b;
    vertices[5] = color.a;
    vertices[6] = 0.0f; // u
    vertices[7] = 0.0f; // v
    
    // Perimeter vertices
    for (int i = 0; i <= segments; i++) {
        float angle = (float)i / segments * 2.0f * 3.14159265359f;
        int idx = (i + 1) * 8; // +1 to skip center, 8 floats per vertex
        vertices[idx + 0] = x + cos(angle) * radius;
        vertices[idx + 1] = y + sin(angle) * radius;
        vertices[idx + 2] = color.r;
        vertices[idx + 3] = color.g;
        vertices[idx + 4] = color.b;
        vertices[idx + 5] = color.a;
        vertices[idx + 6] = 0.0f; // u
        vertices[idx + 7] = 0.0f; // v
    }

    // Duplicate first perimeter vertex at the end for the fan closure
    std::copy_n(&vertices[8], 8, &vertices[(segments + 1) * 8]);
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);
    
    if (filled) {
        glDrawArrays(GL_TRIANGLE_FAN, 0, segments + 2);
    } else {
        // Draw outline using LINE_LOOP to close automatically and avoid artifacts
        // Try enabling line smoothing for nicer result (may be a no-op depending on driver)
        glEnable(GL_LINE_SMOOTH);
        glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
        glLineWidth(1.0f);
        glDrawArrays(GL_LINE_LOOP, 1, segments + 1); // from first perimeter vertex, include duplicate to close
        glDisable(GL_LINE_SMOOTH);
    }
}

void UIRenderer::DrawLine(float x1, float y1, float x2, float y2, float thickness, const Color& color) {
    // Use 8 floats per vertex: x,y, r,g,b,a, u,v (u,v unused)
    float vertices[] = {
        x1, y1, color.r, color.g, color.b, color.a, 0.0f, 0.0f,
        x2, y2, color.r, color.g, color.b, color.a, 0.0f, 0.0f,
    };
    
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);
    
    glLineWidth(thickness);
    glDrawArrays(GL_LINES, 0, 2);
    glLineWidth(1.0f);
}

void UIRenderer::DrawText(const std::string& text, float x, float y, float size, const Color& color) {
    // 'size' is the desired pixel height for the text. Compute scale from the bitmap font's character height
    float scale = size / (float)BitmapFont::FONT_CHAR_HEIGHT;
    float charWidth = BitmapFont::FONT_CHAR_WIDTH * scale;
    float spacing = 1.5f * scale;
    
    float currentX = x;

    // Batch every lit pixel of the string into one buffer and one draw call
    // (one draw per pixel stalls the GPU, especially on macOS).
    std::vector<float> vertices;
    vertices.reserve(text.size() * 20 * 6 * 8);
    auto pushVertex = [&](float vx, float vy) {
        vertices.insert(vertices.end(), {vx, vy, color.r, color.g, color.b, color.a, 0.0f, 0.0f});
    };

    for (char c : text) {
        const uint8_t* bitmap = BitmapFont::GetCharBitmap(c);
        
        // Draw each pixel of the character
        for (int row = 0; row < BitmapFont::FONT_CHAR_HEIGHT; row++) {
            uint8_t rowData = bitmap[row];
            
            for (int col = 0; col < BitmapFont::FONT_CHAR_WIDTH; col++) {
                // Check if pixel is set (from left to right)
                if (rowData & (1 << (4 - col))) {
                    float pixelX = currentX + col * scale;
                    float pixelY = y + row * scale;
                    
                    // Draw a slightly larger rectangle for better visibility
                    float pixelSize = scale * 1.1f;
                    pushVertex(pixelX, pixelY);
                    pushVertex(pixelX + pixelSize, pixelY);
                    pushVertex(pixelX + pixelSize, pixelY + pixelSize);
                    pushVertex(pixelX + pixelSize, pixelY + pixelSize);
                    pushVertex(pixelX, pixelY + pixelSize);
                    pushVertex(pixelX, pixelY);
                }
            }
        }
        
        currentX += charWidth + spacing;
    }

    if (vertices.empty()) return;

    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STREAM_DRAW);
    GLint useTexLoc = glGetUniformLocation(shaderProgram_, "useTexture");
    glUniform1i(useTexLoc, 0);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(vertices.size() / 8));
}

void UIRenderer::DrawTexture(unsigned int textureId, float x, float y, float width, float height, const Color& tint) {
    if (textureId == 0) return;

    // Vertex layout: x, y, r, g, b, a, u, v
    float vertices[] = {
        x, y,            tint.r, tint.g, tint.b, tint.a,      0.0f, 0.0f,
        x + width, y,    tint.r, tint.g, tint.b, tint.a,      1.0f, 0.0f,
        x + width, y + height, tint.r, tint.g, tint.b, tint.a, 1.0f, 1.0f,
        x, y + height,   tint.r, tint.g, tint.b, tint.a,      0.0f, 1.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,
        2, 3, 0
    };

    // Bind texture
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureId);

    // Update vertex buffer
    glBindBuffer(GL_ARRAY_BUFFER, vbo_);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STREAM_DRAW);

    // Update element buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo_);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STREAM_DRAW);

    // Tell shader to use texture
    GLint useTexLoc = glGetUniformLocation(shaderProgram_, "useTexture");
    glUniform1i(useTexLoc, 1);
    GLint texLoc = glGetUniformLocation(shaderProgram_, "uTexture");
    glUniform1i(texLoc, 0); // texture unit 0

    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    // Restore shader state
    glUniform1i(useTexLoc, 0);

    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

void UIRenderer::PushScissor(float x, float y, float width, float height) {
    ScissorRect rect;
    rect.x = (int)x;
    rect.y = windowHeight_ - (int)(y + height);
    rect.width = (int)width;
    rect.height = (int)height;
    
    scissorStack_.push_back(rect);
    
    glEnable(GL_SCISSOR_TEST);
    glScissor(rect.x, rect.y, rect.width, rect.height);
}

void UIRenderer::PopScissor() {
    if (!scissorStack_.empty()) {
        scissorStack_.pop_back();
    }
    
    if (scissorStack_.empty()) {
        glDisable(GL_SCISSOR_TEST);
    } else {
        const auto& rect = scissorStack_.back();
        glScissor(rect.x, rect.y, rect.width, rect.height);
    }
}

} // namespace UI
} // namespace Graphitech
