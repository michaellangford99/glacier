#include "shader.h"

void Shader::generate_imgui_editor()
{	
	//std::string sep_text = "Shader: [vert]:'" + vertex_path + "',[frag]:'" + fragment_path + "'";

	//ImGui::SeparatorText(sep_text.c_str());

	ImGui::PushID(ID);
	if (ImGui::BeginTable("Settings", 4, ImGuiTableFlags_BordersOuter | 
								 /*ImGuiTableFlags_RowBg | */
								 ImGuiTableFlags_BordersInnerH | ImGuiTableFlags_BordersInnerV |
								 ImGuiTableFlags_BordersOuterH |
								 ImGuiTableFlags_Resizable |
								 ImGuiTableFlags_SizingFixedFit))
	{
		ImGui::TableSetupColumn("name", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("value", ImGuiTableColumnFlags_WidthStretch);
		ImGui::TableSetupColumn("min", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableSetupColumn("max", ImGuiTableColumnFlags_WidthFixed);
		ImGui::TableHeadersRow();

		for (int i = 0; i < uniform_list.size(); i++)
		{
			ImGui::TableNextRow();
			ImGui::TableSetColumnIndex(0);
			//ImGui::PushItemWidth(ImGui::CalcItemWidth()/2);
			//ImGui::AlignTextToFramePadding();
			//ImGui::PushMultiItemsWidths(2, ImGui::CalcItemWidth());

			std::string uniform_name = "##" + uniform_list[i].uniform_name;

			ImGui::Text(uniform_list[i].uniform_name.c_str());
			ImGui::TableSetColumnIndex(1);
            ImGui::SetNextItemWidth(-FLT_MIN);

			if ((uniform_list[i].uniform_name.find("color", 0) != -1) && ((uniform_list[i].uniform_type == GL_FLOAT_VEC3) || (uniform_list[i].uniform_type == GL_FLOAT_VEC4)))
			{
				//need a color picker
				if (uniform_list[i].uniform_type == GL_FLOAT_VEC3) ImGui::ColorEdit3(uniform_name.c_str(), glm::value_ptr(uniform_list[i].local_float3));
				if (uniform_list[i].uniform_type == GL_FLOAT_VEC4) ImGui::ColorEdit4(uniform_name.c_str(), glm::value_ptr(uniform_list[i].local_float4));
			}
			else
			{
				switch (uniform_list[i].uniform_type)
				{
				case GL_INT:   ImGui::SliderInt(uniform_name.c_str(), &uniform_list[i].local_int, uniform_list[i].min, uniform_list[i].max, "%d", ImGuiSliderFlags_AlwaysClamp); break;
				case GL_FLOAT: ImGui::SliderFloat(uniform_name.c_str(), &uniform_list[i].local_float, uniform_list[i].min, uniform_list[i].max); break;
				case GL_FLOAT_VEC2: ImGui::SliderFloat2(uniform_name.c_str(), glm::value_ptr(uniform_list[i].local_float2), uniform_list[i].min, uniform_list[i].max); break;
				case GL_FLOAT_VEC3: ImGui::SliderFloat3(uniform_name.c_str(), glm::value_ptr(uniform_list[i].local_float3), uniform_list[i].min, uniform_list[i].max); break;
				case GL_FLOAT_VEC4: ImGui::SliderFloat4(uniform_name.c_str(), glm::value_ptr(uniform_list[i].local_float4), uniform_list[i].min, uniform_list[i].max); break;
				case GL_SAMPLER_2D:
					if (uniform_list[i].local_texture)
					{
						ImTextureID tex_id = (ImTextureID)(void*)(uniform_list[i].local_texture->gl_texture_id);
						float aspect = (float)uniform_list[i].local_texture->height/(float)uniform_list[i].local_texture->width;
						ImGui::ImageButton(("texture##"+std::to_string(i)).c_str(), tex_id, ImVec2(100, 100*aspect));
						if (ImGui::IsItemActive())
						{
							ImGui::BeginTooltip();
							int w = ImGui::GetMainViewport()->Size.x;
							int h = ImGui::GetMainViewport()->Size.y;
							ImGui::Image(tex_id, ImVec2(h*0.9f, h*0.9f*aspect));
							ImGui::EndTooltip();
						}
					}
					break;
				//TODO: add bool, matrix, textures
				
				default: break;
				}

				int min_max_width = 90;


				switch (uniform_list[i].uniform_type)
				{
				case GL_INT:
				case GL_FLOAT:
				case GL_FLOAT_VEC2:
				case GL_FLOAT_VEC3:
				case GL_FLOAT_VEC4: 

					//ImGui::PushID(i);
				
					ImGui::TableSetColumnIndex(2);
					ImGui::SetNextItemWidth(min_max_width);
					//ImGui::SameLine();
					ImGui::InputFloat(("##min"+std::to_string(i)).c_str(), &uniform_list[i].min, 1); 
					ImGui::TableSetColumnIndex(3);
					ImGui::SetNextItemWidth(min_max_width);
					//ImGui::SameLine();
					ImGui::InputFloat(("##max"+std::to_string(i)).c_str(), &uniform_list[i].max, 1); break;
					
					//ImGui::PopID();
				default: break;
				}
			}
		}

		ImGui::EndTable();
	}

	//add new file selection ability / edit path

	if (ImGui::Button("Recompile Shader"))
	{
		std::cout << "Recompiling shader...." << std::endl;

		//save off old uniforms first
		std::vector<uniform_descriptor> old_uniforms = uniform_list;

		delete_shader_program();
		compile_shader_program();
		generate_uniform_table();

		//now try to add them back
		//TODO: this feels a bit icky

		for (auto& u : uniform_list)
		{
			for (auto& old : old_uniforms)
			{
				if (old.uniform_name == u.uniform_name)
				{
					u=old;
				}
			}
		}
	}

	//TODO: finish this deserialization. But not in this class
	//in progress
	if (ImGui::Button("Print Settings"))
	{
		for (int i = 0; i < uniform_list.size(); i++)
		{
			//there should be some form of serialization abstraction in future
			switch (uniform_list[i].uniform_type)
				{
				case GL_INT: std::cout << "(shader)./->set_uniform(\"" << uniform_list[i].uniform_name << "\", " << uniform_list[i].local_int << ");" << std::endl; break;
				case GL_FLOAT: std::cout << "(shader)./->set_uniform(\"" << uniform_list[i].uniform_name << "\", " << uniform_list[i].local_float << ");" << std::endl; break;
				/*case GL_FLOAT_VEC2:
				case GL_FLOAT_VEC3:
				case GL_FLOAT_VEC4: */
				}

		}
	}

	ImGui::PopID();
}

Shader::Shader(std::string _vertex_path, std::string _fragment_path) : vertex_path(_vertex_path), fragment_path(_fragment_path)
{
	//TODO: clean up paths implementation
	//std::string backup_vert_path = vertex_path;
	//std::string backup_frag_path = fragment_path;

	if (navigate_to_source_directory)
	{
		//check if file exists under various subdirectories. HACK.
		vertex_path = "../src/" + vertex_path;
		fragment_path = "../src/" + fragment_path;

		compile_shader_program();
		generate_uniform_table();

		/*	return;
		}
		else
		{
			vertex_path = backup_vert_path;
			fragment_path = backup_frag_path;

			vertex_path = "../../src/" + vertex_path;
			fragment_path = "../../src/" + fragment_path;

			generate_uniform_table();
		}*/
	}
	else
	{
		compile_shader_program();
		generate_uniform_table();
	}
}

bool Shader::compile_shader_program()
{
	std::cout << "Attempting to compile v:" << vertex_path << " f:" << fragment_path << std::endl;

	// 1. retrieve the vertex/fragment source code from filePath
	std::string vertexCode;
	std::string fragmentCode;
	std::ifstream vShaderFile;
	std::ifstream fShaderFile;
	// ensure ifstream objects can throw exceptions:
	vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try
	{
		// open files
		vShaderFile.open(vertex_path.c_str());
		fShaderFile.open(fragment_path.c_str());
		std::stringstream vShaderStream, fShaderStream;
		// read file's buffer contents into streams
		vShaderStream << vShaderFile.rdbuf();
		fShaderStream << fShaderFile.rdbuf();
		// close file handlers
		vShaderFile.close();
		fShaderFile.close();
		// convert stream into string
		vertexCode = vShaderStream.str();
		fragmentCode = fShaderStream.str();
	}
	catch (std::ifstream::failure e)
	{
		std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
		return false;
	}
	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 2. compile shaders
	unsigned int vertex, fragment;
	int success;
	char infoLog[512];

	// vertex Shader
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex, 1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// print compile errors if any
	glGetShaderiv(vertex, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(vertex, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	};

	// similiar for Fragment Shader
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment, 1, &fShaderCode, NULL);
	glCompileShader(fragment);
	// print compile errors if any
	glGetShaderiv(fragment, GL_COMPILE_STATUS, &success);
	if (!success)
	{
		glGetShaderInfoLog(fragment, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
		return false;
	};

	// shader Program
	ID = glCreateProgram();
	glAttachShader(ID, vertex);
	glAttachShader(ID, fragment);
	glLinkProgram(ID);
	// print linking errors if any
	glGetProgramiv(ID, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(ID, 512, NULL, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		return false;
	}

	// delete the shaders as they're linked into our program now and no longer necessary
	glDeleteShader(vertex);
	glDeleteShader(fragment);

	return true;
}

void Shader::delete_shader_program()
{
	glDeleteShader(ID);
}

Shader::~Shader()
{
	delete_shader_program();
}

void Shader::generate_uniform_table()
{
	uniform_list.clear();
	GLint uniform_count = 0;
	glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &uniform_count);

	std::cout << "# Active uniforms: " << uniform_count << "\n";

	for (int i = 0; i < uniform_count; i++)
	{
		char info_string[20];
		GLsizei actual_length;
		GLsizei actual_size;
		GLenum gl_type;
		glGetActiveUniform(ID, i, 20, &actual_length, &actual_size, &gl_type, info_string);
		std::string uniform_name = std::string(info_string, actual_length);
		std::cout << "Uniform - " << uniform_name << "\tsize - " << actual_size << "\ttype - " << gl_type << "\n";

		struct uniform_descriptor desc = { gl_type, uniform_name, actual_size /*intentionally skip local variable initialization*/ };

		desc.min = -1;
		desc.max = 1;

		uniform_list.push_back(desc);
	}
}

void Shader::set_imgui_uniforms()
{
	int texture_index = 0;
	for (int i = 0; i < uniform_list.size(); i++)
	{
		switch (uniform_list[i].uniform_type)
		{
		case GL_BOOL:        glUniform1i(glGetUniformLocation(ID, uniform_list[i].uniform_name.c_str()), uniform_list[i].local_bool); break;
		case GL_INT:        glUniform1i(glGetUniformLocation(ID, uniform_list[i].uniform_name.c_str()), uniform_list[i].local_int);   break;
		case GL_FLOAT:      glUniform1f(glGetUniformLocation(ID, uniform_list[i].uniform_name.c_str()), uniform_list[i].local_float); break;
		case GL_FLOAT_VEC2: glUniform2f(glGetUniformLocation(ID, uniform_list[i].uniform_name.c_str()), uniform_list[i].local_float2.x, uniform_list[i].local_float2.y); break;
		case GL_FLOAT_VEC3: glUniform3f(glGetUniformLocation(ID, uniform_list[i].uniform_name.c_str()), uniform_list[i].local_float3.x, uniform_list[i].local_float3.y, uniform_list[i].local_float3.z); break;
		case GL_FLOAT_VEC4: glUniform4f(glGetUniformLocation(ID, uniform_list[i].uniform_name.c_str()), uniform_list[i].local_float4.x, uniform_list[i].local_float4.y, uniform_list[i].local_float4.z, uniform_list[i].local_float4.w); break;
		case GL_FLOAT_MAT3: glUniformMatrix3fv(glGetUniformLocation(ID, uniform_list[i].uniform_name.c_str()), 1, GL_FALSE, glm::value_ptr(uniform_list[i].local_mat3)); break;
		case GL_FLOAT_MAT4: glUniformMatrix4fv(glGetUniformLocation(ID, uniform_list[i].uniform_name.c_str()), 1, GL_FALSE, glm::value_ptr(uniform_list[i].local_mat4)); break;
		case GL_SAMPLER_2D: 
			if (uniform_list[i].local_texture)
			{	
				GLint backup_active_texture_index = uniform_list[i].local_texture->active_texture_index;
				uniform_list[i].local_texture->save_active_texture_index(texture_index);
				uniform_list[i].local_texture->bind();
				glUniform1i(glGetUniformLocation(ID, uniform_list[i].uniform_name.c_str()), texture_index);
				//restore
				uniform_list[i].local_texture->save_active_texture_index(backup_active_texture_index);
			}
			texture_index++;
			break;
		}
	}
}

void Shader::bind()
{
	glUseProgram(ID);
}

int Shader::get_uniform(std::string name)
{
	for (int i = 0; i < uniform_list.size(); i++)
	{
		if (uniform_list[i].uniform_name == name)
			return i;
	}
	//std::cout << "Could not find " << name << " in shader" << std::endl;
	return -1;
}

void Shader::set_uniform(std::string name, bool value)
{
	int index = get_uniform(name);
	if (index != -1) uniform_list[index].local_bool = value;
}
void Shader::set_uniform(std::string name, int value)
{
	int index = get_uniform(name);
	if (index != -1) uniform_list[index].local_int = value;
}
void Shader::set_uniform(std::string name, float value)
{
	int index = get_uniform(name);
	if (index != -1) uniform_list[index].local_float = value;
}
void Shader::set_uniform(std::string name, glm::vec2 value)
{
	int index = get_uniform(name);
	if (index != -1) uniform_list[index].local_float2 = value;
}
void Shader::set_uniform(std::string name, glm::vec3 value)
{
	int index = get_uniform(name);
	if (index != -1) uniform_list[index].local_float3 = value;
}
void Shader::set_uniform(std::string name, glm::vec4 value)
{
	int index = get_uniform(name);
	if (index != -1) uniform_list[index].local_float4 = value;
}
void Shader::set_uniform(std::string name, glm::mat3 value)
{
	int index = get_uniform(name);
	if (index != -1) uniform_list[index].local_mat3 = value;
}
void Shader::set_uniform(std::string name, glm::mat4 value)
{
	int index = get_uniform(name);
	if (index != -1) uniform_list[index].local_mat4 = value;
}
void Shader::set_uniform(std::string name, texture* value)
{
	int index = get_uniform(name);
	if (index != -1) uniform_list[index].local_texture = value;
}