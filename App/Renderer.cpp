#include "Renderer.h"
#include "Body.h"

Renderer::Renderer(GLFWwindow* win) {
    window = win;
}

void Renderer::newFrame(ImVec4& clear_color) {

    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();

    int display_w, display_h;
    glfwGetFramebufferSize(window, &display_w, &display_h);
    glViewport(0, 0, display_w, display_h);
    glClearColor(clear_color.x * clear_color.w, clear_color.y * clear_color.w, clear_color.z * clear_color.w, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::renderImGUI(bool& show_demo_window, bool& show_another_window, ImVec4& clear_color) {

    ImGuiIO& io = ImGui::GetIO(); (void)io;

    // 1. Show the big demo window (Most of the sample code is in ImGui::ShowDemoWindow()! You can browse its code to learn more about Dear ImGui!).
    if (show_demo_window)
        ImGui::ShowDemoWindow(&show_demo_window);

    // 2. Show a simple window that we create ourselves. We use a Begin/End pair to create a named window.
    {
        static float f = 0.0f;
        static int counter = 0;

        ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

        ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)
        ImGui::Checkbox("Demo Window", &show_demo_window);      // Edit bools storing our window open/close state
        ImGui::Checkbox("Another Window", &show_another_window);

        ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f
        ImGui::ColorEdit3("clear color", (float*)&clear_color); // Edit 3 floats representing a color

        if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
            counter++;
        ImGui::SameLine();
        ImGui::Text("counter = %d", counter);

        ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
        ImGui::End();
    }

    // 3. Show another simple window.
    if (show_another_window)
    {
        ImGui::Begin("Another Window", &show_another_window);   // Pass a pointer to our bool variable (the window will have a closing button that will clear the bool when clicked)
        ImGui::Text("Hello from another window!");
        if (ImGui::Button("Close Me"))
            show_another_window = false;
        ImGui::End();
    }

    // Rendering
    ImGui::Render();
    
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    // Update and Render additional Platform Windows
    // (Platform functions may change the current OpenGL context, so we save/restore it to make it easier to paste this code elsewhere.
    //  For this specific demo app we could also call glfwMakeContextCurrent(window) directly)
    if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        GLFWwindow* backup_current_context = glfwGetCurrentContext();
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
        glfwMakeContextCurrent(backup_current_context);
    }
}

void Renderer::renderSimulation(std::vector<Body> bodies) {
    
    for (auto body : bodies) {
        body.render(this);
    }

    unsigned int fbo;
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);

    GLuint TOF;
    glGenTextures(1, &TOF);
    glBindTexture(GL_TEXTURE_2D, TOF);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 400, 300, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    
    ImGui::Begin("Viewport");
    //ImGui::Text("This is some useful text.");

    //renderCircle(Vector());
    renderCircle(Vector(0, 0), 0.5);

    float view3d_height = ImGui::GetWindowSize().x;
    float view3d_width = ImGui::GetWindowSize().y;
    ImVec2 pos = ImGui::GetCursorScreenPos();
    ImGui::GetWindowDrawList()->AddImage(
        (void*)TOF,
        ImVec2(ImGui::GetCursorScreenPos()),
        ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowSize().x - 15,
            ImGui::GetCursorScreenPos().y + ImGui::GetWindowSize().y - 35),
        ImVec2(0, 1),
        ImVec2(1, 0)

    );
    
    ImGui::End();

    glDeleteFramebuffers(1, &fbo);
    glDeleteTextures(1, &TOF);


}

void Renderer::renderCircle(Vector position, double radius) {
    glBegin(GL_TRIANGLE_FAN);
    glVertex2f(position.x, position.y);
    for (double i = 0; i <= 2 * PI + 0.1; i += 0.1)
    {
        glVertex2f(position.x + sin(i) * radius, position.y + cos(i) * radius);
    }
    glEnd();
}

