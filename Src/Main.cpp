/**
* @file Main.cpp
*/

#include "TitleScene.h"
#include "GLFWEW.h"

int main()
{
	GLFWEW::Window& window = GLFWEW::Window::Instance();
	window.Init(1280, 720, u8"アクションゲーム");

	SceneStack& sceneStack = SceneStack::Instance();
	sceneStack.Push(std::make_shared<TitleScene>());

	while (!window.ShouldClose()) {
		const float deltaTime = window.DeltaTime();
		window.UpdateTimer();
		const float deltaTime = 1.0f / 60.0f;
		sceneStack.Update(deltaTime);
		sceneStack.Render();
		window.SwapBuffers();
	}
}