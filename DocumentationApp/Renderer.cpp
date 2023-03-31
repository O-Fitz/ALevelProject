#include "Renderer.h"
#include "Body.h"
#include "Utilities.h"

Renderer::Renderer(GLFWwindow* win, Simulation* sim) : window(win), simulation(sim){

}

void Renderer::newFrame() {
	// New ImGui frame
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();
	ImGui::DockSpaceOverViewport();

	// Update window size
	int display_w, display_h;
	glfwGetFramebufferSize(window, &display_w, &display_h);
	glViewport(0, 0, display_w, display_h);

	// Clear opengl window buffer
	glClear(GL_COLOR_BUFFER_BIT);
}

void Renderer::renderImGUI() {
	ImGuiIO& io = ImGui::GetIO(); (void)io;


	if (showDataWindow) {
		ImGui::Begin("Data Window");

		ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);

		ImGui::End();
	}

	// RENDER MORE GUI HERE

	if (showGeneralInformation)
		renderGeneralInformationPanel();

	if (showAddObject) 
		renderAddObject();

	// Rendering
	ImGui::Render();

	// Swap buffers to OpenGL
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

	// Allows for ImGUI windows to be outside the main window
	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		GLFWwindow* backup_current_context = glfwGetCurrentContext();
		ImGui::UpdatePlatformWindows();
		ImGui::RenderPlatformWindowsDefault();
		glfwMakeContextCurrent(backup_current_context);
	}
}

void Renderer::renderSimulation() {

	// Setup Projection matrix
	projection = glm::ortho(0.0f, wsize.x, 0.0f, wsize.y, -1.0f, 1.0f);
	glMatrixMode(GL_PROJECTION_MATRIX);
	glLoadMatrixf(glm::value_ptr(projection));

	// Setup ImGui window with no border
	ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
	ImGui::Begin("Viewport");

	// Updates the window position and size
	wsize = ImGui::GetWindowSize();
	pos = ImGui::GetCursorScreenPos();

	// Initialises and binds Frame Buffer
	fb.create_buffers(wsize.x, wsize.y);
	fb.bind();

	// Clear the window
	glClearColor(background.x, background.y, background.z, background.w);
	glClear(GL_COLOR_BUFFER_BIT);

	// Renders Simulation
	std::vector<PBody> n = simulation->getBodies();
	for (int i = 0; i < n.size(); i++) {
		n[i]->render(this);
	}

	// Unbinds Frame buffer
	fb.unbind();

	// Draws the texture linked to the frame buffer to the screen
	ImGui::GetWindowDrawList()->AddImage(
		(void*)fb.get_texture(), ImVec2(ImGui::GetCursorScreenPos()),
		ImVec2(pos.x + wsize.x, pos.y + wsize.y), ImVec2(0, 1), ImVec2(1, 0));

	// End of ImGui window
	ImGui::End();
	ImGui::PopStyleVar(ImGuiStyleVar_WindowPadding);

}

void Renderer::drawCircle(glm::vec2 position, double radius, ImVec4 colour) {
	glBegin(GL_TRIANGLE_FAN);
	// Change colour
	glColor4f(colour.x, colour.y, colour.z, colour.w);
	glVertex2f(position.x, position.y);

	for (double i = 0; i <= 2 * 3.141 + 0.1; i += 0.1)
	{
		// Rotates 0.1 rads round the circle and draws a triangle
		glVertex2f(position.x + sin(i) * radius, position.y + cos(i) * radius);
	}
	glEnd();
}

void Renderer::drawPolygon(glm::vec2 position, std::vector<glm::vec2> vertices, ImVec4 colour) {
	glBegin(GL_TRIANGLE_FAN);
	glColor4f(colour.x, colour.y, colour.z, colour.w);

	for (const glm::vec2& vertex : vertices) {
		glVertex2d(position.x + vertex.x, position.y + vertex.y);
	}

	glEnd();
}

void Renderer::drawLine(glm::vec2 pos1, glm::vec2 pos2, ImVec4 colour) {
	glBegin(GL_LINES);
	glColor4f(colour.x, colour.y, colour.z, colour.w);
	glVertex2f(pos1.x, pos1.y);
	glVertex2f(pos2.x, pos2.y);
	glEnd();
}

ImVec2 Renderer::getWindowSize() {
	return wsize;
}

void Renderer::renderGeneralInformationPanel() {
	ImGui::Begin("General Information", &showGeneralInformation);

	ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Text("Number of objects: %d", simulation->getBodies().size());

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("Clear Simmulation")) {
		simulation->clearSimulation();
	}

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::Checkbox("Collisions", simulation->getCollisionsP());
	ImGui::Spacing();
	ImGui::SliderFloat("Elasticity", simulation->getElasticityP(), 0.0f, 1.0f, "%.3f");
	ImGui::Spacing();
	ImGui::SliderFloat("Simulation Speed", simulation->getSpeed(), -2.5f, 2.5f, "%.2f");

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("Pause/Play")) {
		simulation->pausePlay();
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::End();
}

void Renderer::renderAddObject() {
	ImGui::Begin("Add Object");

	// Body Type dropdown menu
	const char* combo_preview_value = bodyTypes[selectedBodyType];
	if (ImGui::BeginCombo("Object type", combo_preview_value)) {
		for (int n = 0; n < IM_ARRAYSIZE(bodyTypes); n++)
		{
			const bool is_selected = (selectedBodyType == n);
			if (ImGui::Selectable(bodyTypes[n], is_selected))
				selectedBodyType = n;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndCombo();
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	if (selectedBodyType == 0) {
		renderAddBody();
	}
	else if (selectedBodyType == 1) {
		renderAddRigidbody();
	}

	ImGui::End();

}

void Renderer::renderAddBody() {
	static glm::vec2 pos(0, 0);
	static glm::vec2 vel(0, 0);
	static glm::vec2 force(0, 0);
	static float mass = 1;
	static ImVec4 col(0, 0, 0, 255);
	static bool isStatic = false;

	ImGui::InputFloat2("Position", &pos.x);
	ImGui::InputFloat2("Velocity", &vel.x);
	ImGui::InputFloat2("Force", &force.x);
	ImGui::InputFloat("Mass", &mass);
	ImGui::ColorEdit4("Colour", &col.x);
	ImGui::Checkbox("Static", &isStatic);

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("Add Object")) {
		simulation->addBody(Body(pos, vel, force, mass, col, isStatic));
	}
}

void Renderer::renderAddRigidbody() {

	ImGuiIO& io = ImGui::GetIO();

	static glm::vec2 pos(0, 0);
	static glm::vec2 vel(0, 0);
	static glm::vec2 force(0, 0);
	static float mass = 1;
	static ImVec4 col(0, 0, 0, 255);
	static bool isStatic = false;

	static bool useCustomVertices = false;
	static int noVertices = 3;
	static float sideLength = 100;
	static std::vector<glm::vec2> vertexPositions = std::vector<glm::vec2>();

	static int oldVertexNo = 0;
	static float oldSideLength = 0;
	static bool oldCustom = true;
	
	bool changed = oldVertexNo != noVertices || oldSideLength!=sideLength || oldCustom != useCustomVertices;

	// If the state has changed
	if (changed) {
		oldVertexNo = noVertices;
		oldSideLength = sideLength;
		oldCustom = useCustomVertices;
		// Update Vertex positions
		vertexPositions = std::vector<glm::vec2>();
		float r = sideLength / (sqrt(2 * (1 - cos(2 * 3.141 / (float)noVertices))));
		for (float i = 0; i < 2 * 3.141; i += 2 * 3.141 / noVertices) {
			vertexPositions.push_back(r*glm::vec2(-sin(i), cos(i)));
		}

	}

	ImGui::InputFloat2("Position", &pos.x);
	ImGui::InputFloat2("Velocity", &vel.x);
	ImGui::InputFloat2("Force", &force.x);
	ImGui::InputFloat("Mass", &mass);
	ImGui::ColorEdit4("Colour", &col.x);
	ImGui::Checkbox("Static", &isStatic);

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();
	
	ImGui::Text("Vertices: ");
	ImGui::SliderInt("No. of faces", &noVertices, 3, 10);
	ImGui::InputFloat("Side Length", &sideLength);
	ImGui::Checkbox("Use Custom Vertex Positions", &useCustomVertices);

	ImGui::Spacing();

	if (useCustomVertices) {

		// Static variables for custom position calculations
		static bool held = false;
		static std::vector<glm::vec2> relVertices = std::vector<glm::vec2>();
		static int currentPoint = -1;
		static glm::vec2 initialVertexPos = glm::vec2();
		static glm::mat2 transformation = glm::mat2({ {1, 0}, {0, 1} });

		// Find Box size (make it square)
		ImVec2 canvas_sz = ImGui::GetContentRegionAvail();
		if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
		if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
		if (canvas_sz.x < canvas_sz.y) canvas_sz.y = canvas_sz.x;
		else canvas_sz.x = canvas_sz.y;

		if (changed) {
			float max = -std::numeric_limits<float>::max();

			// Find the most extreme point
			for (auto vertex : vertexPositions) {
				max = std::max(max, vertex.x);
				max = std::max(max, vertex.y);
			}

			// Find the transformation matrix
			transformation = (canvas_sz.x)/(2.5f*max) * glm::mat2({ {1, 0}, {0, 1} });

			// Calculate the relative positions of the vertices
			relVertices = std::vector<glm::vec2>();
			for (auto vertex : vertexPositions) {
				relVertices.push_back(transformation*vertex);
			}

		}

		// Create Child window
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));      // Disable padding
		ImGui::PushStyleColor(ImGuiCol_ChildBg, IM_COL32(50, 50, 50, 255));  // Set a background color
		ImGui::BeginChild("canvas", canvas_sz, true, ImGuiWindowFlags_NoMove);
		ImGui::PopStyleColor();
		ImGui::PopStyleVar();

		// Use invisible button for inputs
		ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);

		// Relative position of the mouse to the centre position
		glm::vec2 mousePos = utilities::imvec2_to_vec(ImGui::GetMousePos());
		glm::vec2 windowPos = utilities::imvec2_to_vec(ImGui::GetWindowPos());
		glm::vec2 relMousePos = mousePos - windowPos - 0.5f*utilities::imvec2_to_vec(canvas_sz);

		// If the mouse is hovered or held over the button
		const bool hovered = ImGui::IsItemHovered();
		const bool active = ImGui::IsItemActive();

		if (!held && active) {
			// Has been clicked
			held = true;

			// Find if user has clicked on a point
			// If yes, than the index of that point is stored
			bool clicked = false;
			for (int i = 0; i < relVertices.size(); i++) {
				if (glm::length(relVertices[i] - relMousePos) < 5) {
					currentPoint = i;
					initialVertexPos = relVertices[i];
					clicked = true;
					break;
				}
			}
			// If not, clicked becommes -1
			if (!clicked)
				currentPoint = -1;
		}
		else if (held && active && currentPoint != -1) {
			// The mouse is being held and the user clicked on the vertex
			// Move the relVertex pos to the current mouse pos
			relVertices[currentPoint] = relMousePos;
		}
		else if (held && !hovered) {
			// Mouse has moved off the box
			// Move the vertex back to original position
			held = false;
			if (currentPoint != -1)
				relVertices[currentPoint] = initialVertexPos;
		}
		else if (held && !active) {
			// User has released the mouse
			held = false;

			// Validation
			bool convex = true;
			bool sign = false;
			// Checks that the shape is convex
			for (int i = 0; i < relVertices.size(); i++) {
				glm::vec2 p1 = relVertices[i];
				glm::vec2 p2 = relVertices[(i + 1) % relVertices.size()];
				glm::vec2 p3 = relVertices[(i + 2) % relVertices.size()];
				// Calculates which direction the vertex "bends" around
				double cross = (p2.x - p1.x) * (p3.y - p2.y) - (p2.y - p1.y) * (p3.x - p2.x);
				if (i == 0) {
					// If this is the first iteration
					sign = cross > 0.0;
				}
				else if ((cross > 0.0) != sign) {
					// If at any time, the vertex bends in a different direction
					// the shape is concave
					convex = false;
					break;
				}
			}

			// Update vertexPositions
			if (convex) {
				vertexPositions = std::vector<glm::vec2>();
				for (auto relvertex : relVertices) {
					vertexPositions.push_back(glm::inverse(transformation) * relvertex);
				}
			}
			else if (currentPoint != -1) {
				relVertices[currentPoint] = initialVertexPos;
			}
		}

		// Draw vertices:
		ImDrawList* draw_list = ImGui::GetWindowDrawList();

		for (int i = 0; i < relVertices.size(); i++) {
			ImVec2 pos1 = utilities::vec_to_imvec2(relVertices[i]+windowPos+ 0.5f * utilities::imvec2_to_vec(canvas_sz));
			ImVec2 pos2 = utilities::vec_to_imvec2(relVertices[(i+1)%relVertices.size()] + windowPos + 0.5f * utilities::imvec2_to_vec(canvas_sz));
			draw_list->AddLine(pos1, pos2, IM_COL32(col.x, col.y, col.z, 255));
			draw_list->AddCircleFilled(pos1, 2, ImColor(255, 0, 0, 255));
		}
		draw_list->AddCircleFilled(ImVec2(windowPos.x + 0.5f * canvas_sz.x, windowPos.y + 0.5f * canvas_sz.y), 2, ImColor(255, 255, 0, 255));

		ImGui::EndChild();
	}

	ImGui::Spacing();
	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("Add Object")) {
		simulation->addRigidbody(Rigidbody(vertexPositions, pos, vel, force, mass, col, isStatic));
	}

}