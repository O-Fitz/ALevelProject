#include "Renderer.h"
#include "Body.h"
#include "Utilities.h"

Renderer::Renderer(GLFWwindow* win, Simulation* sim) : window(win), simulation(sim){

	saveWindow = ImGui::FileBrowser(ImGuiFileBrowserFlags_EnterNewFilename | ImGuiFileBrowserFlags_CreateNewDir);
	saveWindow.SetTitle("Save");
	saveWindow.SetTypeFilters({ ".phys" });

	loadWindow = ImGui::FileBrowser();
	loadWindow.SetTitle("Load");
	loadWindow.SetTypeFilters({ ".phys" });

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

	// RENDER MORE GUI HERE

	if (showGeneralInformation)
		renderGeneralInformationPanel();

	if (showAddObject) 
		renderAddObject();

	if (showObjectInformation)
		renderObjectInformation();

	if (showHelpMenu)
		renderHelpMenu();

	if (showSaveWindow)
		renderSaveWindow();

	if (showLoadWindow)
		renderLoadWindow();

	renderMenuBar();

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
	ImGui::Begin("Viewport", NULL, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

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

	// Invisible button for inputs:
	ImGui::InvisibleButton(" ", wsize, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
	// Handle inputs
	handleSimulationInputs();


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
	else if (selectedBodyType == 2) {
		renderAddRectangle();
	}
	else if (selectedBodyType == 3) {
		renderAddSquare();
	}
	else if (selectedBodyType == 4) {
		renderAddCircle();
	}
	else if (selectedBodyType == 5) {
		renderAddSoftbody();
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
		static glm::mat2 transformation = glm::mat2({ {1, 0}, {0, -1} });

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
			transformation = (canvas_sz.x)/(2.5f*max) * glm::mat2({ {1, 0}, {0, -1} });

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

void Renderer::renderAddRectangle() {
	static glm::vec2 pos(0, 0);
	static glm::vec2 vel(0, 0);
	static glm::vec2 force(0, 0);
	static float mass = 1;
	static ImVec4 col(0, 0, 0, 255);
	static bool isStatic = false;
	static float width = 200;
	static float height = 100;

	ImGui::InputFloat2("Position", &pos.x);
	ImGui::InputFloat2("Velocity", &vel.x);
	ImGui::InputFloat2("Force", &force.x);
	ImGui::InputFloat("Mass", &mass);
	ImGui::ColorEdit4("Colour", &col.x);
	ImGui::Checkbox("Static", &isStatic);

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::InputFloat("Width", &width);
	ImGui::InputFloat("Height", &height);

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("Add Object")) {
		simulation->addRectangle(Rectangle(width, height, pos, vel, force, mass, col, isStatic));
	}
}

void Renderer::renderAddSquare() {
	static glm::vec2 pos(0, 0);
	static glm::vec2 vel(0, 0);
	static glm::vec2 force(0, 0);
	static float mass = 1;
	static ImVec4 col(0, 0, 0, 255);
	static bool isStatic = false;
	static float sideLength = 100;

	ImGui::InputFloat2("Position", &pos.x);
	ImGui::InputFloat2("Velocity", &vel.x);
	ImGui::InputFloat2("Force", &force.x);
	ImGui::InputFloat("Mass", &mass);
	ImGui::ColorEdit4("Colour", &col.x);
	ImGui::Checkbox("Static", &isStatic);

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::InputFloat("Side Length", &sideLength);

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("Add Object")) {
		simulation->addSquare(Square(sideLength, pos, vel, force, mass, col, isStatic));
	}
}

void Renderer::renderAddCircle() {
	static glm::vec2 pos(0, 0);
	static glm::vec2 vel(0, 0);
	static glm::vec2 force(0, 0);
	static float mass = 1;
	static ImVec4 col(0, 0, 0, 255);
	static bool isStatic = false;
	static float radius = 100;

	ImGui::InputFloat2("Position", &pos.x);
	ImGui::InputFloat2("Velocity", &vel.x);
	ImGui::InputFloat2("Force", &force.x);
	ImGui::InputFloat("Mass", &mass);
	ImGui::ColorEdit4("Colour", &col.x);
	ImGui::Checkbox("Static", &isStatic);

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::InputFloat("Radius", &radius);

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("Add Object")) {
		simulation->addCircle(Circle(radius, pos, vel, force, mass, col, isStatic));
	}
}

void Renderer::renderAddSoftbody() {
	static glm::vec2 pos(0, 0);
	static glm::vec2 vel(0, 0);
	static glm::vec2 force(0, 0);
	static float mass = 1;
	static ImVec4 col(0, 0, 0, 255);
	static bool isStatic = false;
	static int noPoints = 10;
	static float radius = 100;
	static float springConstant = 100;
	static float dampeningFactor = 1;

	ImGui::InputFloat2("Position", &pos.x);
	ImGui::InputFloat2("Velocity", &vel.x);
	ImGui::InputFloat2("Force", &force.x);
	ImGui::InputFloat("Mass", &mass);
	ImGui::ColorEdit4("Colour", &col.x);
	ImGui::Checkbox("Static", &isStatic);

	ImGui::Spacing();
	ImGui::Spacing();

	ImGui::SliderInt("No. Points", &noPoints, 6, 15);
	ImGui::InputFloat("Radius", &radius);
	if (ImGui::InputFloat("Spring Constant", &springConstant)) { // Validates so that spring constant is positive
		if (springConstant <= 0) {
			springConstant = 1.0f;
		}
	};
	if (ImGui::InputFloat("Dampening Factor", &dampeningFactor)) { // Validates so that dampening factor is positive
		if (dampeningFactor <= 0) {
			dampeningFactor = 1.0f;
		}
	}

	ImGui::Spacing();
	ImGui::Spacing();

	if (ImGui::Button("Add Object")) {
		simulation->addSoftbody(Softbody(pos, vel, force, mass, col, isStatic, noPoints, radius, springConstant, dampeningFactor));
	}
}

void Renderer::handleSimulationInputs() {

	// If the mouse is hovered or held over the button
	const bool hovered = ImGui::IsItemHovered();
	const bool active = ImGui::IsItemActive();

	// Find mouse positions
	glm::vec2 mousePos = utilities::imvec2_to_vec(ImGui::GetMousePos());
	glm::vec2 windowPos = utilities::imvec2_to_vec(ImGui::GetWindowPos());
	float scroll = ImGui::GetScrollMaxY();
	glm::vec2 relMousePos = glm::mat2(1, 0, 0, -1) * (mousePos - windowPos - glm::vec2(0, wsize.y+scroll));

	static bool held = false;
	static glm::vec2 initialPos = glm::vec2(0, 0);
	static glm::vec2 relBodyPos = glm::vec2(0, 0);

	// New mouse click
	if (!held && active) {
		held = true;
		// Find focused body
		focusedBody = simulation->getClickedObject(relMousePos);
		// If a body was clicked on
		if (focusedBody != nullptr) {
			// Set the initial pos to be the position of the body
			initialPos = focusedBody->getPosition();
			// Find the relative body position
			relBodyPos = initialPos - relMousePos;
			// Pause the simulation
			simulation->pause();
		}
	}
	// Mouse being held
	else if (held && active) {
		// If a body is being dragged
		if (focusedBody != nullptr) {
			// Set the position of the body to be the
			// relative mouse position + relative body position
			focusedBody->setPosition(relMousePos+relBodyPos);
		}
	}
	// Mouse moved out of viewport
	else if (held && !hovered) {
		// If a body was clicked on
		if (focusedBody != nullptr) {
			// Set the body position to be its initial position
			focusedBody->setPosition(initialPos);
		}
		// set held to false
		held = false;
	}
	// Mouse released
	else if (held && !active) {
		// Set held to false
		held = false;
	}
}

void Renderer::renderObjectInformation() {

	ImGui::Begin("Object Information", &showObjectInformation);

	if (focusedBody != nullptr) {


		ImGui::InputFloat2("Position", &(focusedBody->getPositionP()->x));
		ImGui::InputFloat2("Velocity", &focusedBody->getVelocityP()->x);
		ImGui::InputFloat2("Force", &focusedBody->getForceP()->x);
		ImGui::InputFloat("Mass", focusedBody->getMassP());
		ImGui::ColorEdit4("Colour", &focusedBody->getColourP()->x);
		ImGui::Checkbox("Static", focusedBody->getStaticP());

	}
	else {
		ImGui::Text("No Object has been selected");
	}
	ImGui::End();

}

void Renderer::renderHelpMenu() {
	static int selected = 0;
	
	ImGui::Begin("Help", &showHelpMenu);

	// Select From List of menus
	ImGui::BeginChild("left pane", ImVec2(150, 0), true);

	// Menu Items
	char label1[] = "General Information Window";
	if (ImGui::Selectable(label1, selected == 0))
		selected = 0;

	char label2[] = "Example Help Window";
	if (ImGui::Selectable(label2, selected == 1))
		selected = 1;

	ImGui::EndChild();

	ImGui::SameLine();

	// Text for each time
	ImGui::BeginGroup();

	ImGui::BeginChild("");
	if (selected == 0) {

		ImGui::Text("General Information Window");
		ImGui::Separator();
		std::string text = std::string("The General Information window contains information about the simulation. It contains:\n")+
			std::string("\n- Framerate\n- The Number of Objects\n- A button to clear the simulation\n- A toggle to turn on or off collisions\n")+
			std::string("- A slide to control how elasic the collisions are\n- A slide to control the speed of the simulation\n")+
			std::string("- And a button to pause the simulation.");
		ImGui::Text(text.c_str());
	}
	else if (selected == 1) {
		ImGui::Text("Example Help Window");
		ImGui::Separator();
		ImGui::Text("This is an example help window.");
	}

	ImGui::EndChild();
	ImGui::EndGroup();

	ImGui::End();
}

void Renderer::renderSaveWindow() {
	// Display window
	saveWindow.Display();

	// If user has selected path
	if (saveWindow.HasSelected()) {
		// Find path
		std::string filepath = saveWindow.GetSelected().string();
		// Check if path contains ".phys" at the end
		if (filepath.find(".phys") != filepath.size()-5) {
			// If it doesn't, add it
			filepath += ".phys";
		}
		// Save the simulation
		simulation->save(filepath);
		// Hide the save window
		showSaveWindow = false;
		// Clear the selection.
		saveWindow.ClearSelected();
	}

}

void Renderer::renderLoadWindow() {
	// Display window
	loadWindow.Display();

	// If user has selected path
	if (loadWindow.HasSelected()) {
		// Find path
		std::string filepath = loadWindow.GetSelected().string();
		// Save the simulation
		simulation->load(filepath);
		// Hide the save window
		showLoadWindow = false;
		// Clear the selection.
		loadWindow.ClearSelected();
	}
}

void Renderer::renderMenuBar() {
	if (ImGui::BeginMainMenuBar()) {
		// File Menu
		if (ImGui::BeginMenu("File")) {
			// Save button
			if (ImGui::MenuItem("Save")) {
				// Open the save window
				showSaveWindow = true;
				saveWindow.Open();
			}

			// Load button
			if (ImGui::MenuItem("Load")) {
				// Open the load window
				showLoadWindow = true;
				loadWindow.Open();
			}

			ImGui::EndMenu();
		}

		//  Open windows
		if (ImGui::BeginMenu("View")) {
			
			// Options for each different window
			ImGui::MenuItem("General Information", "", &showGeneralInformation);
			ImGui::MenuItem("Add Object", "", &showAddObject);
			ImGui::MenuItem("Object Information", "", &showObjectInformation);

			ImGui::EndMenu();
		}

		// Help window
		if (ImGui::MenuItem("Help", "", showHelpMenu)) {
			showHelpMenu = true;
		}

		ImGui::EndMainMenuBar();
	}
}