#include <memory>
#include "../mgl/mgl.hpp"

////////////////////////////////////////////////////////////////////////// TANGRAM PIECE

class TangramPiece {
public:
    TangramPiece(GLuint vao, GLsizei indexCount, const glm::mat4& transform, const glm::vec4& color)
        : vao(vao), indexCount(indexCount), modelMatrix(transform), pieceColor(color) {
    }

    void draw(mgl::ShaderProgram* shader, GLint matrixId, GLint colorId) const {
        glBindVertexArray(vao);
        glUniformMatrix4fv(matrixId, 1, GL_FALSE, glm::value_ptr(modelMatrix));
        glUniform4fv(colorId, 1, glm::value_ptr(pieceColor));
        glDrawElements(GL_TRIANGLES, indexCount, GL_UNSIGNED_BYTE, reinterpret_cast<GLvoid*>(0));
        glBindVertexArray(0);
    }

private:
    GLuint vao;
    GLsizei indexCount;
    glm::mat4 modelMatrix;
    glm::vec4 pieceColor;
};

////////////////////////////////////////////////////////////////////////// MYAPP

class MyApp : public mgl::App {
public:
    MyApp() = default;
    ~MyApp() override = default;

    void initCallback(GLFWwindow* win) override;
    void displayCallback(GLFWwindow* win, double elapsed) override;
    void windowCloseCallback(GLFWwindow* win) override;
    void windowSizeCallback(GLFWwindow* win, int width, int height) override;

private:
    const GLuint POSITION = 0;
    std::unique_ptr<mgl::ShaderProgram> Shaders = nullptr;
    GLint MatrixId;
    GLint ShapeColorId;

    GLuint triangleVAO, squareVAO, parallelogramVAO;

    std::unique_ptr<TangramPiece> smallTriangle1;
    std::unique_ptr<TangramPiece> smallTriangle2;
    std::unique_ptr<TangramPiece> mediumTriangle;
    std::unique_ptr<TangramPiece> largeTriangle1;
    std::unique_ptr<TangramPiece> largeTriangle2;
    std::unique_ptr<TangramPiece> square;
    std::unique_ptr<TangramPiece> parallelogram;

    void createShaderProgram();
	void createBufferObjects();
    void createTangramPieces();
	void destroyBufferObjects();
    void drawScene();

};

//////////////////////////////////////////////////////////////////////// SHADERs

void MyApp::createShaderProgram() {
    Shaders = std::make_unique<mgl::ShaderProgram>();
    Shaders->addShader(GL_VERTEX_SHADER, "clip-vs.glsl");
    Shaders->addShader(GL_FRAGMENT_SHADER, "clip-fs.glsl");

    Shaders->addAttribute(mgl::POSITION_ATTRIBUTE, POSITION);
    Shaders->addUniform("Matrix");
    Shaders->addUniform("ShapeColor");

    Shaders->create();

    MatrixId = Shaders->Uniforms["Matrix"].index;
    ShapeColorId = Shaders->Uniforms["ShapeColor"].index;
}

//////////////////////////////////////////////////////////////////// VAOs & VBOs

typedef struct {
    GLfloat XYZW[4];
} Vertex;

const Vertex Triangle[] = {
    {{0.0f, 0.0f, 0.0f, 1.0f}},
    {{0.2f, 0.0f, 0.0f, 1.0f}},
    {{0.2f, 0.2f, 0.0f, 1.0f}}
};

const Vertex Square[] = {
	{{0.0f, 0.0f, 0.0f, 1.0f}},
	{{0.2f, 0.0f, 0.0f, 1.0f}},
	{{0.2f, 0.2f, 0.0f, 1.0f}},
	{{0.0f, 0.2f, 0.0f, 1.0f}}
};

const Vertex Parallelogram[] = {
	{{0.0f, 0.0f, 0.0f, 1.0f}},
	{{0.2f, 0.2f, 0.0f, 1.0f}},
	{{0.2f, 0.4f, 0.0f, 1.0f}},
	{{0.0f, 0.2f, 0.0f, 1.0f}}
};

const GLubyte TriangleIndices[] = { 0, 1, 2 };
const GLubyte SquareIndices[] = { 0, 1, 2, 0, 2, 3 };
const GLubyte ParallelogramIndices[] = { 0, 1, 2, 0, 2, 3 };

void MyApp::createBufferObjects() {
    glGenVertexArrays(1, &triangleVAO);
    glBindVertexArray(triangleVAO);
    {
        GLuint triangleVBO, triangleEBO;
        glGenBuffers(1, &triangleVBO);
        glGenBuffers(1, &triangleEBO);
        glBindBuffer(GL_ARRAY_BUFFER, triangleVBO);

        {
            glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle, GL_STATIC_DRAW);
            glEnableVertexAttribArray(POSITION);
            glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
                reinterpret_cast<GLvoid*>(0));
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, triangleEBO);

        {
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(TriangleIndices), TriangleIndices, GL_STATIC_DRAW);
        }
    }
	glGenVertexArrays(1, &squareVAO);
	glBindVertexArray(squareVAO);
	{
		GLuint squareVBO, squareEBO;
		glGenBuffers(1, &squareVBO);
		glGenBuffers(1, &squareEBO);
		glBindBuffer(GL_ARRAY_BUFFER, squareVBO);

		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Square), Square, GL_STATIC_DRAW);
			glEnableVertexAttribArray(POSITION);
			glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				reinterpret_cast<GLvoid*>(0));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, squareEBO);

		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(SquareIndices), SquareIndices, GL_STATIC_DRAW);
		}
	}
	glGenVertexArrays(1, &parallelogramVAO);
	glBindVertexArray(parallelogramVAO);
	{
		GLuint parallelogramVBO, parallelogramEBO;
		glGenBuffers(1, &parallelogramVBO);
		glGenBuffers(1, &parallelogramEBO);
		glBindBuffer(GL_ARRAY_BUFFER, parallelogramVBO);
		{
			glBufferData(GL_ARRAY_BUFFER, sizeof(Parallelogram), Parallelogram, GL_STATIC_DRAW);
			glEnableVertexAttribArray(POSITION);
			glVertexAttribPointer(POSITION, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex),
				reinterpret_cast<GLvoid*>(0));
		}
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, parallelogramEBO);
		{
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(ParallelogramIndices), ParallelogramIndices, GL_STATIC_DRAW);
		}
	}
    glBindVertexArray(0);
}

void MyApp::destroyBufferObjects() {
    glDeleteVertexArrays(1, &triangleVAO);
    glDeleteVertexArrays(1, &squareVAO);
    glDeleteVertexArrays(1, &parallelogramVAO);
}

////////////////////////////////////////////////////////////////////////// SCENE

const glm::mat4 I(1.0f);

void MyApp::createTangramPieces() {
    glm::vec4 colorRed(0.89f, 0.32f, 0.19f, 1.0f);
    glm::vec4 colorGreen(0.13f, 0.62f, 0.13f, 1.0f);
    glm::vec4 colorPink(0.98f, 0.38f, 0.60f, 1.0f);
    glm::vec4 colorBlue(0.23f, 0.56f, 0.98f, 1.0f);
    glm::vec4 colorTeal(0.13f, 0.62f, 0.62f, 1.0f);
    glm::vec4 colorPurple(0.80f, 0.62f, 0.98f, 1.0f);
    glm::vec4 colorOrange(1.0f, 0.62f, 0.0f, 1.0f);

    largeTriangle1 = std::make_unique<TangramPiece>(
        triangleVAO, 3,
        glm::translate(I, glm::vec3(-0.3f, 0.2f, 0.0f)) *
        glm::rotate(I, glm::radians(225.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
        glm::scale(I, glm::vec3(2.0f, 2.0f, 1.0f)),
        colorPink
    );

	largeTriangle2 = std::make_unique<TangramPiece>(
		triangleVAO, 3,
		glm::translate(I, glm::vec3(0.1f, 0.2f, 0.0f)) *
		glm::rotate(I, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(I, glm::vec3(2.0f, 2.0f, 1.0f)),
		colorBlue
	);

	square = std::make_unique<TangramPiece>(
		squareVAO, 6,
		glm::translate(I, glm::vec3(-0.647f, -0.3f, 0.0f)) *
		glm::rotate(I, glm::radians(45.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(I, glm::vec3(1.0f, 1.0f, 1.0f)),
		colorGreen
	);

	smallTriangle1 = std::make_unique<TangramPiece>(
		triangleVAO, 3,
		glm::translate(I, glm::vec3(-0.3f, -0.365f, 0.0f)) *
		glm::rotate(I, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(I, glm::vec3(1.0f, 1.0f, 1.0f)),
		colorRed
	);

	smallTriangle2 = std::make_unique<TangramPiece>(
		triangleVAO, 3,
		glm::translate(I, glm::vec3(0.238f, 0.058f, 0.0f)) *
		glm::rotate(I, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(I, glm::vec3(1.0f, 1.0f, 1.0f)),
		colorTeal
	);
    
	mediumTriangle = std::make_unique<TangramPiece>(
		triangleVAO, 3,
		glm::translate(I, glm::vec3(0.24f, 0.056f, 0.0f)) *
		glm::rotate(I, glm::radians(180.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(I, glm::vec3(1.4142f, 1.4142f, 1.0f)),
		colorPurple
	);

	parallelogram = std::make_unique<TangramPiece>(
		parallelogramVAO, 6,
		glm::translate(I, glm::vec3(0.38f, -0.084f, 0.0f)) *
		glm::rotate(I, glm::radians(135.0f), glm::vec3(0.0f, 0.0f, 1.0f)) *
		glm::scale(I, glm::vec3(1.0f, 1.0f, 1.0f)),
		colorOrange
	);
}

void MyApp::drawScene() {
    Shaders->bind();
	largeTriangle1->draw(Shaders.get(), MatrixId, ShapeColorId);
	largeTriangle2->draw(Shaders.get(), MatrixId, ShapeColorId);
	square->draw(Shaders.get(), MatrixId, ShapeColorId);
	smallTriangle1->draw(Shaders.get(), MatrixId, ShapeColorId);
	smallTriangle2->draw(Shaders.get(), MatrixId, ShapeColorId);
	mediumTriangle->draw(Shaders.get(), MatrixId, ShapeColorId);
	parallelogram->draw(Shaders.get(), MatrixId, ShapeColorId);
	Shaders->unbind();
}

////////////////////////////////////////////////////////////////////// CALLBACKS

void MyApp::initCallback(GLFWwindow* win) {
    createBufferObjects();
    createShaderProgram();
	createTangramPieces();
}

void MyApp::windowCloseCallback(GLFWwindow* win) { destroyBufferObjects(); }

void MyApp::windowSizeCallback(GLFWwindow* win, int winx, int winy) {
    glViewport(0, 0, winx, winy);
}

void MyApp::displayCallback(GLFWwindow* win, double elapsed) { drawScene(); }

/////////////////////////////////////////////////////////////////////////// MAIN

int main(int argc, char* argv[]) {
    mgl::Engine& engine = mgl::Engine::getInstance();
    engine.setApp(new MyApp());
    engine.setOpenGL(4, 6);
    engine.setWindow(600, 600, "Hello Modern 2D World", 0, 1);
    engine.init();
    engine.run();
    exit(EXIT_SUCCESS);
}

//////////////////////////////////////////////////////////////////////////// END
