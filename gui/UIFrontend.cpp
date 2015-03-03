#include "UIFrontend.h"

#define OFFSETOF(TYPE, ELEMENT) ((size_t)&(((TYPE *)0)->ELEMENT))

static bool mousePressed[2] = { false, false };

static int shader_handle, vert_handle, frag_handle;
static int texture_location, proj_mtx_location;
static int position_location, uv_location, colour_location;
static size_t vbo_max_size = 20000;
static unsigned int vbo_handle, vao_handle;

static void ImImpl_RenderDrawLists(ImDrawList** const cmd_lists, int cmd_lists_count)
{
	if(cmd_lists_count == 0) return;

	glEnable(GL_BLEND);
	glBlendEquation(GL_FUNC_ADD);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_SCISSOR_TEST);
	glActiveTexture(GL_TEXTURE0);

	const float width = ImGui::GetIO().DisplaySize.x;
	const float height = ImGui::GetIO().DisplaySize.y;
	const float ortho_projection[4][4] =
	{
		{ 2.0f/width,	0.0f,			0.0f,		0.0f },
		{ 0.0f,			2.0f/-height,	0.0f,		0.0f },
		{ 0.0f,			0.0f,			-1.0f,		0.0f },
		{ -1.0f,		1.0f,			0.0f,		1.0f },
	};
	glUseProgram(shader_handle);
	glUniform1i(texture_location, 0);
	glUniformMatrix4fv(proj_mtx_location, 1, GL_FALSE, &ortho_projection[0][0]);

	size_t total_vtx_count = 0;
	for(int n = 0; n < cmd_lists_count; n++)
		total_vtx_count += cmd_lists[n]->vtx_buffer.size();
	glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
	size_t neededBufferSize = total_vtx_count * sizeof(ImDrawVert);
	if(neededBufferSize > vbo_max_size)
	{
		vbo_max_size = neededBufferSize + 5000;  // Grow buffer
		glBufferData(GL_ARRAY_BUFFER, vbo_max_size, NULL, GL_STREAM_DRAW);
	}

	// Copy and convert all vertices into a single contiguous buffer
	unsigned char* buffer_data = (unsigned char*)glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
	if (!buffer_data)
		return;
	for(int n = 0; n < cmd_lists_count; n++)
	{
		const ImDrawList* cmd_list = cmd_lists[n];
		memcpy(buffer_data, &cmd_list->vtx_buffer[0], cmd_list->vtx_buffer.size() * sizeof(ImDrawVert));
		buffer_data += cmd_list->vtx_buffer.size() * sizeof(ImDrawVert);
	}
	glUnmapBuffer(GL_ARRAY_BUFFER);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(vao_handle);

	int cmd_offset = 0;
	for(int n = 0; n < cmd_lists_count; n++)
	{
		const ImDrawList* cmd_list = cmd_lists[n];
		int vtx_offset = cmd_offset;
		const ImDrawCmd* pcmd_end = cmd_list->commands.end();
		for (const ImDrawCmd* pcmd = cmd_list->commands.begin(); pcmd != pcmd_end; pcmd++)
		{
			glBindTexture(GL_TEXTURE_2D, (GLuint)(intptr_t)pcmd->texture_id);
			glScissor((int)pcmd->clip_rect.x, (int)(height - pcmd->clip_rect.w), (int)(pcmd->clip_rect.z - pcmd->clip_rect.x), (int)(pcmd->clip_rect.w - pcmd->clip_rect.y));
			glDrawArrays(GL_TRIANGLES, vtx_offset, pcmd->vtx_count);
			vtx_offset += pcmd->vtx_count;
		}
		cmd_offset = vtx_offset;
	}

	// Restore modified state
	glBindVertexArray(0);
	glUseProgram(0);
	glDisable(GL_SCISSOR_TEST);
	glBindTexture(GL_TEXTURE_2D, 0);
}

static const char* ImImpl_GetClipboardTextFn()
{
	return Context::getInstance().getClipboardContent().c_str();
}

static void ImImpl_SetClipboardTextFn(const char* text)
{
	Context::getInstance().setClipboardContent(text);
}

UIFrontend::UIFrontend()
{

}

bool create_project = false;
bool empty_proj = false;
bool change_core = false;
static float _px, _py, _pz;
static float _rx = 0.0f, _ry = 0.0f, _rz = 0.0f, _rw = 0.0f;
static float _sx = 1.0f, _sy = 1.0f, _sz = 1.0f;
static float _psx = 1.0f, _psy = 1.0f, _psz = 1.0f;
static float step = 0.5f;
static char selection_name[256] = "Object0";
static int cull_face = 0;
static bool use_depth_mask = true;
static int physics_type = 0;
static int collision_shape = 0;
static float _mass = 1.0f;
static float _friction = 0.1f;
static float _restitution = 0.1f;

std::string folder = "";
Project proj;
RenderableNode* current = 0;

void UIFrontend::load(const std::vector<std::string>& args)
{
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	gl::init();
	gl::clearColor(vec4(0.15, 0.15, 0.15, 1.0));
	glShadeModel(GL_FLAT);
	MaterialLibrary::getInstance().initialize();

	const GLchar *vertex_shader =
		"#version 330\n"
		"uniform mat4 ProjMtx;\n"
		"in vec2 Position;\n"
		"in vec2 UV;\n"
		"in vec4 Color;\n"
		"out vec2 Frag_UV;\n"
		"out vec4 Frag_Color;\n"
		"void main()\n"
		"{\n"
		"	Frag_UV = UV;\n"
		"	Frag_Color = Color;\n"
		"	gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
		"}\n";

	const GLchar* fragment_shader =
		"#version 330\n"
		"uniform sampler2D Texture;\n"
		"in vec2 Frag_UV;\n"
		"in vec4 Frag_Color;\n"
		"out vec4 Out_Color;\n"
		"void main()\n"
		"{\n"
		"	Out_Color = Frag_Color * texture( Texture, Frag_UV.st);\n"
		"}\n";

	shader_handle = glCreateProgram();
	vert_handle = glCreateShader(GL_VERTEX_SHADER);
	frag_handle = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(vert_handle, 1, &vertex_shader, 0);
	glShaderSource(frag_handle, 1, &fragment_shader, 0);
	glCompileShader(vert_handle);
	glCompileShader(frag_handle);
	glAttachShader(shader_handle, vert_handle);
	glAttachShader(shader_handle, frag_handle);
	glLinkProgram(shader_handle);

	texture_location = glGetUniformLocation(shader_handle, "Texture");
	proj_mtx_location = glGetUniformLocation(shader_handle, "ProjMtx");
	position_location = glGetAttribLocation(shader_handle, "Position");
	uv_location = glGetAttribLocation(shader_handle, "UV");
	colour_location = glGetAttribLocation(shader_handle, "Color");

	glGenBuffers(1, &vbo_handle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
	glBufferData(GL_ARRAY_BUFFER, vbo_max_size, 0, GL_DYNAMIC_DRAW);

	glGenVertexArrays(1, &vao_handle);
	glBindVertexArray(vao_handle);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_handle);
	glEnableVertexAttribArray(position_location);
	glEnableVertexAttribArray(uv_location);
	glEnableVertexAttribArray(colour_location);

	glVertexAttribPointer(position_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, pos));
	glVertexAttribPointer(uv_location, 2, GL_FLOAT, GL_FALSE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, uv));
	glVertexAttribPointer(colour_location, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(ImDrawVert), (GLvoid*)OFFSETOF(ImDrawVert, col));
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	// IMGUI init
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = 1.0f / 60.0f;
	io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
	io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
	io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
	io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
	io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
	io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
	io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
	io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
	io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
	io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
	io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
	io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
	io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
	io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
	io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
	io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
	io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

	io.RenderDrawListsFn = ImImpl_RenderDrawLists;
	io.SetClipboardTextFn = ImImpl_SetClipboardTextFn;
	io.GetClipboardTextFn = ImImpl_GetClipboardTextFn;

	unsigned char* pixels;
	int width, height;
	io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);

	GLuint tex_id;
	glGenTextures(1, &tex_id);
	glBindTexture(GL_TEXTURE_2D, tex_id);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	io.Fonts->TexID = (void *)(intptr_t)tex_id;

	pos = vec3(0, 0, 16);
	x_rot = -24.620016;
	y_rot = 30.619991;

	// add standard cube
	m_objects.push_back(std::make_shared<RenderableNode>());
	auto node = m_objects[m_objects.size()-1].get();
	auto renderer = node->getRenderer();
	renderer->loadCube(1.0);
	renderer->setMaterial(MaterialLibrary::getInstance().getDefault());

	current = node;
	resetTransform(vec3(0.0), quat(0.0, 0.0, 0.0, 0.0), vec3(1.0));

	node->setName("Cube0");
	m_scene.addChild(node);
	updateProperties(node);
}

void UIFrontend::render()
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheel = 0;
	mousePressed[0] = mousePressed[1] = false;

	vec2 window = Context::getInstance().getWindowSize();
	vec2 display = Context::getInstance().getFramebufferSize();
	io.DisplaySize = ImVec2(display.x, display.y);

	static double time = 0.0f;
	const double current_time = glfwGetTime();
	io.DeltaTime = (float)(current_time - time);
	time = current_time;

	vec2 mouse = Context::getInstance().getCursorPosition();
	mouse.x *= (float)display.x / window.x;
	mouse.y *= (float)display.y / window.y;
	io.MousePos = ImVec2((float)mouse.x, (float)mouse.y);
	io.MouseDown[0] = mousePressed[0] || glfwGetMouseButton(Context::getInstance().getWindow(), GLFW_MOUSE_BUTTON_LEFT) != 0;
	io.MouseDown[1] = mousePressed[1] || glfwGetMouseButton(Context::getInstance().getWindow(), GLFW_MOUSE_BUTTON_RIGHT) != 0;
	ImGui::NewFrame();

	// draw 3d viewport
	gl::viewport(vec4(0, 0, io.DisplaySize.x, io.DisplaySize.y));
	gl::clear();

	glEnable(GL_CULL_FACE);
	glDisable(GL_BLEND);
	glCullFace(GL_BACK);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	if(current)
	{
		current->setCollisionType((RenderableNode::Collision)physics_type);
		current->setMass(_mass);
		current->setFriction(_friction);
		current->setRestitution(_restitution);
		current->setColliderScale(vec3(_psx, _psy, _psz));
		current->setColliderType((RenderableNode::Collider)collision_shape);

		MeshRenderer* renderer = current->getRenderer();
		renderer->setPosition(vec3(_px, _py, _pz));
		renderer->setRotation(quat(_rx, _ry, _rz, _rw));
		renderer->setScale(vec3(_sx, _sy, _sz));

		if(cull_face == 0) renderer->setCullFace(GL_BACK);
		else if(cull_face == 1) renderer->setCullFace(GL_FRONT);
		else renderer->setCullFace(GL_NONE);

		current->setName(std::string(selection_name));
	}

	mat4 perspective = glm::perspective(45.0f, (float)m_dimension.x / m_dimension.y, 0.1f, 10000.0f);

	gl::update(perspective);
	gl::translate(vec3(pos.x, pos.y, -pos.z));
	gl::rotate(x_rot, vec3(1, 0, 0));
	gl::rotate(y_rot, vec3(0, 1, 0));

	m_camera.setPosition(pos);
	m_camera.setModelViewProjection(gl::getModelViewProjection());
	m_scene.draw(&m_camera);

	int lines = 8;
	for(int i = -lines; i <= lines; i++)
	{
		gl::setColor(vec4(0.7, 0.7, 0.7, 1.0));
		gl::begin();
		gl::drawLine(vec3(i, 0, -lines), vec3(i, 0, lines));
		gl::drawLine(vec3(-lines, 0, i), vec3(lines, 0, i));
		gl::end();
	}

	if(current)
	{
		glDisable(GL_DEPTH_TEST);
		vec3 origin = vec3(_px, _py, _pz);

		gl::setColor(vec4(0.836, 0.0, 0.0, 1.0));
		gl::begin();
		gl::drawVector(origin, origin + vec3(3.0, 0.0, 0.0), 0.7, 0.1);
		gl::end();

		gl::setColor(vec4(0.0, 0.836, 0.0, 1.0));
		gl::begin();
		gl::drawVector(origin, origin + vec3(0.0, 0.0, 3.0), 0.7, 0.1);
		gl::end();

		gl::setColor(vec4(0.0, 0.0, 0.836, 1.0));
		gl::begin();
		gl::drawVector(origin, origin + vec3(0.0, 3.0, 0.0), 0.7, 0.1);
		gl::end();
		gl::setColor(vec4(0.7, 0.7, 0.7, 1.0));

		gl::begin();
		gl::drawWireCube(origin, vec3(0.5));
		gl::end();
	}

	glEnable(GL_BLEND);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// gui values
	bool open = true;
	float alpha = 0.8;

	// Top bar
	ImGui::Begin("Main", &open, ImVec2(m_dimension.x, 90), alpha, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		ImGui::Text("3D Game Engine");
		if(ImGui::Button("Open Project"))
		{
			auto filename = FileIO::browseFile("Project file (*.vproj)\0*.vproj\0\0", "vproj");
			if(filename != "")
			{
				proj.load(filename);
			}
		}
		ImGui::SameLine();
		if(ImGui::Button("Create Project"))
		{
			create_project = true;
		}
		ImGui::SameLine();
		if(ImGui::Button("Save level"))
		{
			std::string level_file = "level0.lvl";
			SceneManager::getInstance().exportScene(proj.getPath()+"/levels", level_file, &m_scene);
		}
		ImGui::SameLine();
		if(ImGui::Button("Run"))
		{
			change_core = true;
		}
		ImGui::SameLine();
		if(ImGui::Button("Close"))
		{
			close();
		}

		ImGui::Separator();
	}
	ImGui::End();

	// Hierachy window / Scene window
	ImGui::Begin("Scene Window", &open, ImVec2(350, m_dimension.y), alpha, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	{
		ImGui::Text("Scene Window");
		ImGui::Separator();

		std::string title = proj.getTitle();
		std::string script = proj.getScript();
		std::string name_info = "Project: "+ ((title == "") ? "None" : title);
		std::string script_info = "Script: " + ((script == "") ? "None" : script);
		ImGui::TextWrapped(name_info.c_str());
		ImGui::TextWrapped(script_info.c_str());
		ImGui::Separator();

		ImGui::Text("Create");
		if(ImGui::CollapsingHeader("Create Geometry", 0, true, true))
		{
			if(ImGui::Button("Add Empty"))
			{

			}
			if(ImGui::Button("Add Cube"))
			{
				m_objects.push_back(std::make_shared<RenderableNode>());
				auto node = m_objects[m_objects.size()-1].get();
				auto renderer = node->getRenderer();
				renderer->loadCube(1.0);
				renderer->setMaterial(MaterialLibrary::getInstance().getDefault());

				current = node;
				resetTransform(vec3(0.0), quat(0.0, 0.0, 0.0, 0.0), vec3(1.0));

				node->setName("Cube0");
				node->setType(RenderableNode::Type::CUBE);
				m_scene.addChild(node);
				updateProperties(node);
			}
			if(ImGui::Button("Add Plane"))
			{
				m_objects.push_back(std::make_shared<RenderableNode>());
				auto node = m_objects[m_objects.size()-1].get();
				auto renderer = node->getRenderer();
				renderer->loadPlane(1.0);
				renderer->setMaterial(MaterialLibrary::getInstance().getDefault());

				current = node;
				resetTransform(vec3(0.0), quat(0.0, 0.0, 0.0, 0.0), vec3(1.0));

				node->setName("Plane0");
				node->setType(RenderableNode::Type::PLANE);
				m_scene.addChild(node);
				updateProperties(node);
			}
			if(ImGui::Button("Add Mesh"))
			{
				auto filename = FileIO::browseFile("Wavefront OBJ file (*.obj)\0*.obj\0\0", "obj");
				if(filename != "")
				{
					m_objects.push_back(std::make_shared<RenderableNode>());
					auto node = m_objects[m_objects.size()-1].get();
					auto renderer = node->getRenderer();
					renderer->load(filename);
					renderer->setMaterial(MaterialLibrary::getInstance().getDefault());

					current = node;
					resetTransform(vec3(0.0), quat(0.0, 0.0, 0.0, 0.0), vec3(1.0));

					node->setName("Mesh0");
					node->setType(RenderableNode::Type::MESH);
					node->setMeshPath(filename);
					m_scene.addChild(node);
					updateProperties(node);
				}
			}
		}

		ImGui::Separator();
		if(ImGui::CollapsingHeader("Hierachy", 0, true, true))
		{
			SceneNode::NodePointerList nodes = m_scene.getChildren();

			for(auto i = 0; i < nodes.size(); i++)
			{
				RenderableNode* node = dynamic_cast<RenderableNode*>(nodes[i]);
				if(node)
				{
					if(ImGui::Selectable(node->getName().c_str(), node->getIsSelected()))
					{
						current = node;
						auto r = node->getRenderer();
						resetTransform(r->getPosition(), quat(0.0, 0.0, 0.0, 0.0), r->getScale());
						updateProperties(node);
						break;
					}
				}
			}
		}
	}
	ImGui::End();

	// Properties window
	ImGui::Begin("Properties", &open, ImVec2(350, m_dimension.y), alpha, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
	{
		ImGui::Text("Properties");
		ImGui::Separator();

		if(current)
		{
			ImGui::InputText("Name (id)", selection_name, 256);

			ImGui::Separator();
			if(ImGui::CollapsingHeader("Transform", 0, true, true))
			{
				ImGui::Text("Position");
				ImGui::InputFloat("px", &_px, step);
				ImGui::InputFloat("py", &_py, step);
				ImGui::InputFloat("pz", &_pz, step);

				ImGui::Text("Rotation");
				ImGui::InputFloat("rx", &_rx, step);
				ImGui::InputFloat("ry", &_ry, step);
				ImGui::InputFloat("rz", &_rz, step);
				ImGui::InputFloat("rw", &_rw, step);

				ImGui::Text("Scale");
				ImGui::InputFloat("sx", &_sx, step);
				ImGui::InputFloat("sy", &_sy, step);
				ImGui::InputFloat("sz", &_sz, step);
			}
			if(ImGui::CollapsingHeader("Material", 0, true, true))
			{
				ImGui::Combo("Cull Face", &cull_face, "Back\0Front\0None\0\0");
				ImGui::Checkbox("Use depth mask", &use_depth_mask);
			}
			if(ImGui::CollapsingHeader("Physics", 0, true, true))
			{
				ImGui::Combo("Physics Type", &physics_type, "No Collision\0Static\0Dynamic\0\0");
				ImGui::Combo("Collision Shape", &collision_shape, "Box\0Sphere\0Mesh\0\0");
				ImGui::InputFloat("Mass", &_mass, 0, 0.5f);
				ImGui::InputFloat("Friction", &_friction, 0, 0.5f);
				ImGui::InputFloat("Restitution", &_restitution, 0, 0.5f);

				if(collision_shape == 0)
				{
					ImGui::Text("Shape scale");
					ImGui::InputFloat("x", &_psx, step);
					ImGui::InputFloat("y", &_psy, step);
					ImGui::InputFloat("z", &_psz, step);
				}
				else if(collision_shape == 1)
				{
					ImGui::InputFloat("Shape scale", &_psx, step);
				}
			}

			if(ImGui::Button("Duplicate"))
			{
				m_objects.push_back(std::make_shared<RenderableNode>());
				auto node = m_objects[m_objects.size()-1].get();
				auto name = current->getName();

				//TODO FIX, crash on remove
				auto renderer = node->getRenderer();
				*renderer = *(current->getRenderer());

				current = node;
				resetTransform(node->getRenderer()->getPosition(), quat(0.0, 0.0, 0.0, 0.0), node->getRenderer()->getScale());
				node->setName(name + "(Duplicate)");
				m_scene.addChild(node);
				updateProperties(node);
			}

			if(ImGui::Button("Remove"))
			{
				m_scene.removeChild(current);
				for(auto i = 0; i < m_objects.size(); i++)
				{
					if(m_objects[i]->getId() == current->getId())
					{
						m_objects.erase(m_objects.begin() + i);
					}
				}

				SceneNode::NodePointerList nodes = m_scene.getChildren();
				if(nodes.size() != 0)
				{
					RenderableNode* node = dynamic_cast<RenderableNode*>(nodes[0]);
					if(node)
					{
						current = node;
						auto r = node->getRenderer();
						resetTransform(r->getPosition(), quat(0.0, 0.0, 0.0, 0.0), r->getScale());
						updateProperties(current);
					}
					else
					{
						current = 0;
					}
				}
				else
				{
					current = 0;
				}
			}
		}
		else
		{
			ImGui::Text("Select an object");
		}
	}
	ImGui::End();


	if(create_project)
	{
		ImGui::Begin("Project tool", &create_project, ImVec2(400, 120), alpha, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);
		ImGui::Text("Create a new Project");
		ImGui::Separator();
		if(ImGui::Button("Select folder"))
		{
			folder = FileIO::browseFolder();
		}
		ImGui::Spacing();

		ImGui::TextWrapped(folder == "" ? "No folder selected" : folder.c_str());
		static char buffer[256] = "Project1";
		ImGui::InputText("Project name", buffer, 256);
		ImGui::Spacing();
		ImGui::Separator();

		if(ImGui::Button("Create"))
		{
			Project proj0;
			proj0.create(std::string(buffer));
			proj0.save(folder, empty_proj);
			create_project = false;
			folder = "";
		}

		ImGui::SameLine();
		ImGui::Checkbox("Empty", &empty_proj);

		ImGui::End();
	}

	setWindowPos("Main", ImVec2(-2, -6));
	setWindowPos("Project tool", ImVec2(m_dimension.x * 0.5 - 200, m_dimension.y * 0.5 - 60));
	setWindowPos("Scene Window", ImVec2(0, 90));
	setWindowPos("Properties", ImVec2(m_dimension.x - 350, 90));

	ImGui::Render();

	if(change_core)
	{
		Engine engine;
		std::vector<std::string> args;
		args.push_back(proj.getProjectFile());

		Context::getInstance().changeCore(&engine, args);
		glDisable(GL_FRAMEBUFFER_SRGB);
		gl::clearColor(vec4(0.15, 0.15, 0.15, 1.0));
		Context::getInstance().setCursorVisibility(true);
		change_core = false;
	}
}

void UIFrontend::resetTransform(const vec3& position, const quat& rotation, const vec3& scale)
{
	_px = position.x, _py = position.y, _pz = position.z;
	_rx = rotation.x, _ry = rotation.y, _rz = rotation.z, _rw = rotation.w;
	_sx = scale.x, _sy = scale.y, _sz = scale.z;
}

void UIFrontend::updateProperties(SceneNode* node_ptr)
{
	RenderableNode* node = dynamic_cast<RenderableNode*>(node_ptr);
	if(node)
	{
		auto renderer = node->getRenderer();
		auto size = (sizeof(selection_name) / sizeof(char));
		strncpy(selection_name, node->getName().c_str(), size - 1);
		selection_name[size - 1] = '\0';

		cull_face = renderer->getCullFace();
		if(cull_face == GL_NONE) cull_face = 2;
		else if(cull_face == GL_FRONT) cull_face = 1;
		else if(cull_face == GL_BACK) cull_face = 0;

		use_depth_mask = renderer->getDepthMaskUsage();

		// TEMP
		physics_type = (int)node->getCollisionType();
		collision_shape = (int)node->getColliderType();
		PhysicsInfo info = node->getPhysicsInfo();
		_mass = info.mass;
		_friction = info.friction;
		_restitution = info.restitution;
		_psx = info.scale.x;
		_psy = info.scale.y;
		_psz = info.scale.z;
	}
}

void UIFrontend::clear()
{
	if(vao_handle) glDeleteVertexArrays(1, &vao_handle);
	if(vbo_handle) glDeleteBuffers(1, &vbo_handle);
	glDetachShader(shader_handle, vert_handle);
	glDetachShader(shader_handle, frag_handle);
	glDeleteShader(vert_handle);
	glDeleteShader(frag_handle);
	glDeleteProgram(shader_handle);

	ImGui::Shutdown();
	MaterialLibrary::getInstance().free();
	Console::log("Released context");
}

void UIFrontend::onKeyInput(int key, int scancode, int action, int mods)
{
	ImGuiIO& io = ImGui::GetIO();
	if(action == GLFW_PRESS) io.KeysDown[key] = true;
	if (action == GLFW_RELEASE) io.KeysDown[key] = false;
	io.KeyCtrl = (mods & GLFW_MOD_CONTROL) != 0;
	io.KeyShift = (mods & GLFW_MOD_SHIFT) != 0;
}

void UIFrontend::onCharInput(unsigned int c)
{
	if(c > 0 && c < 0x10000) ImGui::GetIO().AddInputCharacter((unsigned short)c);
}

void UIFrontend::onMouseButton(int button, int action, int mods)
{
	m_button = button;
	m_action = action;

	if(action == GLFW_PRESS && button >= 0 && button < 2)
		mousePressed[button] = true;
}

void UIFrontend::onMouseMovement(double x, double y)
{
 	double xOffset = x-last_x;
	double yOffset = y-last_y;
	last_x = x;
	last_y = y;

	if(m_action == GLFW_PRESS && x > 350 &&  x < m_dimension.x - 350 && !create_project)
	{
		if(m_button == 0)
		{
			x_rot += (float) 0.005f * yOffset;
			y_rot += (float) 0.005f * xOffset;
		}
		else if(m_button == 2)
		{
		 	pos.x += (float) 0.02f * xOffset;
		 	pos.y -= (float) 0.02f * yOffset;
		}
	}
}

void UIFrontend::onScroll(double xOffset, double yOffset)
{
	ImGuiIO& io = ImGui::GetIO();
	io.MouseWheel += (float)yOffset;
	pos.z -= yOffset;
}

void UIFrontend::onResize(int width, int height)
{
	m_viewport[2] = width;
	m_viewport[3] = height;
	m_dimension = vec2(width, height);
}
