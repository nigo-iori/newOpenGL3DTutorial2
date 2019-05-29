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
	* GLFWとGLEWのラッパークラス
	*/
	class Window
	{
	public:

		// キーが押されているかの状態
		enum PressState
		{
			released,
			press1st,
			pressed,
		};

		// キーの情報を格納する構造体
		PressState pressState[GLFW_KEY_LAST + 1];
		// キーの状態を初期化する
		void InitPressState();
		// キーの状態を更新する
		void UpdatePressState();

		int Width = 800;
		int Height = 600;

		static Window& Instance();
		bool Init(int w, int h, const char* title);
		bool ShouldClose() const;
		void SwapBuffers() const;

		/// 経過時間を計る
		void InitTimer();		/// GLFWの計測時間を初期化
		void UpdateTimer();		/// 経過時間の測定
		double DeltaTime() const;		/// UpdateTimer関数で計測した経過時間を取得

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
		double previousTime = 0;		/// 前回の時間を記録する
		double deltaTime = 0;		/// 経過時間を記録する
		GamePad gamepad;
	};
} /// namespace GLFWEW

#endif /// GLFWEW_H_INCLUDED