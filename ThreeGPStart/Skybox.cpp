#include "Skybox.h"
#include "Mesh.h"

void Skybox::loadCubemap(std::vector<std::string> faces_filenames)
{
	CreateSkybox();

	Helpers::ImageLoader texLoader;
	
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	int width, height, nrChannels;
	for (unsigned int i = 0; i < faces_filenames.size(); i++)
	{
		texLoader.Load(faces_filenames[i]);
		if (texLoader.GetData() != nullptr)
		{
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, texLoader.Width(), texLoader.Height(), 0, GL_RGBA, GL_UNSIGNED_BYTE, texLoader.GetData());
		}
		else
		{
			std::cout << "Cubemap texture failed to load at path: " << faces_filenames[i] << std::endl;
		}
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	Helpers::CheckForGLError();
}

void Skybox::LoadSkybox(std::string filename)
{
	Helpers::ModelLoader loader;
	if (!loader.LoadFromFile(filename))
		std::cout << "skybox failed to load";
}

GLuint Skybox::GetVAO()
{
	return m_VAO;
}

GLuint Skybox::GetTexID()
{
	return textureID;
}

GLuint Skybox::GetProgram()
{
	return m_program;
}

void Skybox::CreateProgram()
{
	// Create a new program (returns a unqiue id)
	m_program = glCreateProgram();

	// Load and create vertex and fragment shaders
	GLuint vertex_shader{ Helpers::LoadAndCompileShader(GL_VERTEX_SHADER, vertShader) };
	GLuint fragment_shader{ Helpers::LoadAndCompileShader(GL_FRAGMENT_SHADER, fragShader) };
	if (vertex_shader == 0 || fragment_shader == 0)
		return;

	// Attach the vertex shader to this program (copies it)
	glAttachShader(m_program, vertex_shader);

	// Attach the fragment shader (copies it)
	glAttachShader(m_program, fragment_shader);

	// Done with the originals of these as we have made copies
	glDeleteShader(vertex_shader);
	glDeleteShader(fragment_shader);

	// Link the shaders, checking for errors
	if (!Helpers::LinkProgramShaders(m_program))
		return;

	Helpers::CheckForGLError();
}

void Skybox::CreateSkybox()
{

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * skyboxVertices.size(), skyboxVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	Helpers::CheckForGLError();

	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	Helpers::CheckForGLError();
	 
	
}
