#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/random.hpp>

inline static float Map(float p_value, float p_low1, float p_high1, float p_low2, float p_high2)
{
	return (p_low2 + (p_value - p_low1) * (p_high2 - p_low2) / (p_high1 - p_low1));
}

class Star
{
public:
	Star()
	{
		m_Position = glm::linearRand(glm::vec3(-640.0f, -360.0f, 0.0f), glm::vec3(640.0f, 360.0f, 1280.0f));
		m_PreviousZ = m_Position.z;
	}

	void Update()
	{
		m_Position.z -= 20.0f;
		if (m_Position.z < 1.0f)
		{
			m_Position.x = glm::linearRand(-640.0f, 640.0f);
			m_Position.y = glm::linearRand(-360.0f, 360.0f);
			m_Position.z = 1280.0f;
			m_PreviousZ = m_Position.z;
		}
	}

	void Draw()
	{
		float SX =  Map(m_Position.x / m_Position.z, 0.0f, 1.0f, 0.0f, 1280.0f);
		float SY = Map(m_Position.y / m_Position.z, 0.0f, 1.0f, 0.0f, 720.0f);

		float PSX = Map(m_Position.x / m_PreviousZ, 0.0f, 1.0f, 0.0f, 1280.0f);
		float PSY = Map(m_Position.y / m_PreviousZ, 0.0f, 1.0f, 0.0f, 720.0f);

		float Size = Map(m_Position.z, 1.0f, 1280.0f, 8.0, 0.0f);
		glPointSize(Size);

		float Alpha = Map(m_Position.z, 1.0f, 1280.0f, 1.0f, 0.0f);
		glColor4f(1.0f, 1.0f, 1.0f, Alpha);

		glBegin(GL_TRIANGLES);
		{
			glVertex2f(SX, SY + (Size / 2.0f));
			glVertex2f(PSX, PSY);
			glVertex2f(SX, SY - (Size / 2.0f));
		}
		glEnd();

		glBegin(GL_POINTS);
		{
			glVertex2f(SX, SY);
		}
		glEnd();

		m_PreviousZ = m_Position.z;
	}

private:
	glm::vec3 m_Position;
	float m_PreviousZ;
};

int main(int argc, char** argv)
{
	srand(time(nullptr));

	glfwInit();
	glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
	glfwWindowHint(GLFW_SAMPLES, 4);
	GLFWwindow* Window = glfwCreateWindow(1280, 720, "Starfield", nullptr, nullptr);
	glfwMakeContextCurrent(Window);
	glfwSwapInterval(2);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_POINT_SMOOTH);
	glPointSize(8.0f);

	glMatrixMode(GL_PROJECTION);
	glOrtho(-640.0, 640.0, -360.0, 360.0, 0.0, 1.0);
	glMatrixMode(GL_MODELVIEW);

	glColor4f(1.0f, 1.0f, 1.0f, 0.5f);

	uint32_t StarCount = 10000;
	Star* Stars = new Star[StarCount];

	while (!glfwWindowShouldClose(Window))
	{
		glfwPollEvents();
		glClear(GL_COLOR_BUFFER_BIT);
		for (uint32_t i = 0; i < StarCount; i++)
		{
			Stars[i].Update();
			Stars[i].Draw();
		}
		glfwSwapBuffers(Window);
	}

	glfwTerminate();
	return 0;
}