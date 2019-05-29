//#pragma once
/**
* @@file GLFWEW.h
*
*/
#ifndef GLFWEW_H_INCLUDED
#define GLFWEW_H_INCLUDED
#include <GL/glew.h>
#include "GamePad.h"
#include <GLFW/glfw3.h>
#include <Windows.h>
#include <vadefs.h>
#include <vector>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>

namespace GLFWEW
{


	/**
	* GLFW��GLEW�̃��b�p�[�N���X
	*/
	class Window
	{
	public:

		// �L�[��������Ă��邩�̏��
		enum PressState
		{
			released,
			press1st,
			pressed,
		};

		// �L�[�̏����i�[����\����
		PressState pressState[GLFW_KEY_LAST + 1];
		// �L�[�̏�Ԃ�����������
		void InitPressState();
		// �L�[�̏�Ԃ��X�V����
		void UpdatePressState();

		int Width = 800;
		int Height = 600;

		static Window& Instance();
		bool Init(int w, int h, const char* title);
		bool ShouldClose() const;
		void SwapBuffers() const;

		/// �o�ߎ��Ԃ��v��
		void InitTimer();		/// GLFW�̌v�����Ԃ�������
		void UpdateTimer();		/// �o�ߎ��Ԃ̑���
		double DeltaTime() const;		/// UpdateTimer�֐��Ōv�������o�ߎ��Ԃ��擾

		bool IsKeyPressed(int key) const;
		bool KeyDown(int key) const;
		const GamePad& GetGamePad() const;

	private:
		Window();
		~Window();
		Window(const Window&) = delete;
		Window& operator = (const Window&) = delete;
		void UpdateGamePad();

		bool isGLFWInitialized = false;
		bool isInitialized = false;
		GLFWwindow* window = nullptr;
		double previousTime = 0;		/// �O��̎��Ԃ��L�^����
		double deltaTime = 0;		/// �o�ߎ��Ԃ��L�^����
		GamePad gamepad;
	};
} /// namespace GLFWEW

#endif /// GLFWEW_H_INCLUDED