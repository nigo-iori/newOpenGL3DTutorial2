/**
* @FILE GLFWEW.cpp
*/
#include "GLFWEW.h"
#include <iostream>

/// GLFW��GLEW�����b�v���邽�߂̖��O���
namespace GLFWEW
{

	/**
	* GLFW����̃G���[�񍐂���������
	*
	* @param error�@�G���[�ԍ�
	* @param desc�@�G���[�̓��e
	*/
	void ErrorCallback(int error, const char* desc)
	{
		std::cerr << "ERROR: " << desc << std::endl;
	}

	/**
	* �V���O���g���C���X�^���X���擾����
	*
	* @return�@Window�̃V���O���g���C���X�^���X
	*/
	Window& Window::Instance()
	{
		static Window instance;
		return instance;
	}

	/**
	* �R���X�g���N�^
	*/
	Window::Window()
	{
	}

	/**
	* �f�X�g���N�^
	*/
	Window::~Window()
	{
		if (isGLFWInitialized)
		{
			glfwTerminate();
		}
	}

	/**
	* GLFW/GLEW�̏�����
	*
	* @param w�@�E�B���h�E�̕`��͈͂̕�(�s�N�Z��)
	* @param h�@�E�B���h�E�̕`��͈͂̍���(�s�N�Z��)
	* @param title�@�E�B���h�E�^�C�g��(UTF-8��0�I�[������)
	*
	* @retval true�@����������
	* @retval false�@���������s
	*/
	bool Window::Init(int w, int h, const char* title)
	{
		if (isInitialized)
		{
			std::cerr << "ERRPR: GLFWEW�͂��łɏ���������Ă��܂��B" << std::endl;
			return false;
		}
		if (!isGLFWInitialized)
		{
			///�@�R�[���o�b�N��GLFW�ɐݒ肷��
			glfwSetErrorCallback(ErrorCallback);
			if (glfwInit() != GL_TRUE)
			{
				return false;
			}
			isGLFWInitialized = true;
		}

		if (!window)
		{
			window =
				glfwCreateWindow(w, h, title, nullptr, nullptr);	/// width, height, Title, FullScrean
			if (!window)
			{
				return false;
			}

			/// �w�肵��window��OpenGL�̐ݒ��n��
			glfwMakeContextCurrent(window);
		}

		/// GLEW�̏�����
		if (glewInit() != GLEW_OK)
		{
			std::cerr << "ERROR: GLEW�̏������Ɏ��s���܂����B" << std::endl;
			return false;
		}

		/// OpenGL�̏����R���\�[���E�B���h�E�֏o�͂���
		const GLubyte* renderer = glGetString(GL_RENDERER);
		std::cout << "Renderer: " << renderer << std::endl;
		const GLubyte* version = glGetString(GL_VERSION);
		std::cout << "Version: " << version << std::endl;
		const GLubyte* SLversion = glGetString(GL_SHADING_LANGUAGE_VERSION);
		std::cout << "Shading Langage Version: " << SLversion << std::endl;

		isInitialized = true;
		return true;
	}

	/**
	* �E�B���h�E�����ׂ������ׂ�
	*
	* @retval true�@����
	* @retval false�@���Ȃ�
	*/
	bool Window::ShouldClose() const
	{
		return glfwWindowShouldClose(window) != 0;
	}

	/**
	* �t�����g�o�b�t�@�ƃo�b�N�o�b�t�@��؂�ւ���
	*/
	void Window::SwapBuffers() const
	{
		glfwPollEvents(); /// GLFW��OS����̃C�x���g����������
		glfwSwapBuffers(window); /// GLFW�̓_�u���o�b�t�@
	}



	/**
	* �L�[��������Ă��邩���ׂ�
	*
	* @param key�@���ׂ����L�[�E�R�[�h
	*
	* @retval true�@�L�[��������Ă���
	* @retval false�@�L�[��������Ă��Ȃ�
	*
	* �g�p�ł���L�[�E�R�[�h�̈ꗗ��glfw3.h�ɂ���܂�(�ukeyboard�v�Ō���)
	*/
	bool Window::IsKeyPressed(int key) const
	{
		return (glfwGetKey(window, key) == GLFW_PRESS);
	}

	/**
	* �L�[�������ꂽ1�t���[���𔻒�
	*
	* @param key	���ׂ����L�[�R�[�h
	*
	* @retval true	�L�[�������ꂽ1�t���[����
	* @retval false	�L�[�������ꂽ1�t���[���ڈȊO
	*/
	bool Window::KeyDown(int key) const
	{

		if (key < 0 || key > GLFW_KEY_LAST)
		{
			return false;
		}
		return pressState[key] == PressState::press1st;
	}
	/**
	* �L�[�̏�Ԃ�����������
	*/
	void Window::InitPressState()
	{
		for (int i = 0; i < GLFW_KEY_LAST; ++i)
		{
			pressState[i] = PressState::released;
		}
	}

	/**
	* �L�[�̏�Ԃ��X�V����
	*/
	void Window::UpdatePressState()
	{

		// �L�[�̏�Ԃ̍X�V
		for (int i = 0; i < GLFW_KEY_LAST; ++i)
		{
			const bool pressed = glfwGetKey(window, i) == GLFW_PRESS;
			if (pressed)
			{
				if (pressState[i] == PressState::released)
				{
					pressState[i] = PressState::press1st;
				}
				else if (pressState[i] == PressState::press1st)
				{
					pressState[i] = PressState::pressed;
				}
			}
			else if (pressState[i] != PressState::released)
			{
				pressState[i] = PressState::released;
			}
		}


	}

	/**
	* �^�C�}�[������������
	*/
	void Window::InitTimer()
	{
		glfwSetTime(0.0);		/// �v�����Ԃ��������BglfwSetTime(�����̒l���v�����Ԃɐݒ�)
		previousTime = 0.0;
		deltaTime = 0.0;
	}

	/**
	* �^�C�}�[���X�V����
	*/
	void Window::UpdateTimer()
	{
		/// �o�ߎ��Ԃ��v��
		const double currentTime = glfwGetTime();
		deltaTime = currentTime - previousTime;
		previousTime = currentTime;

		/// �o�ߎ��Ԃ������Ȃ肷���Ȃ��悤�ɒ���
		const float upperLimit = 0.25f;		/// �o�ߎ��ԂƂ��ċ��e�������
		if (deltaTime > upperLimit)
		{
			deltaTime = 1.0f / 60.0f;
		}
		UpdateGamePad();
	}

	/**
	*  �o�ߎ��Ԃ��擾����
	*
	* @return ���O��2���UpdateTimer()�Ăяo���̊ԂɌo�߂�������
	*/
	double Window::DeltaTime() const
	{
		return deltaTime;
	}

	/**
	 * �Q�[���p�b�h�̏�Ԃ��擾����.
	 *
	 * @return �Q�[���p�b�h�̏��.
	 */
	const GamePad& Window::GetGamePad() const
	{
		return gamepad;
	}

	 /**
	  * �Q�[���p�b�h�̃A�i���O���͑��uID.
	  +
	  * @note ������XBOX360�Q�[���p�b�h�.
	  */
	enum GAMEPAD_AXES
	{
		GAMEPAD_AXES_LEFT_X,  ///< ���X�e�B�b�N��X��.
		GAMEPAD_AXES_LEFT_Y,  ///< ���X�e�B�b�N��Y��.
		GAMEPAD_AXES_TRIGGER, ///< �A�i���O�g���K�[.
		GAMEPAD_AXES_RIGHT_Y, ///< �E�X�e�B�b�N��Y��.
		GAMEPAD_AXES_RIGHT_X, ///< �E�X�e�B�b�N��X��.
	};

	/**
	 * �Q�[���p�b�h�̃f�W�^�����͑��uID.
	 *
	 * @note Xbox360�Q�[���p�b�h����.
	 */
	enum GAMEPAD_BUTTON {
		GAMEPAD_BUTTON_A, ///< A�{�^��.
		GAMEPAD_BUTTON_B, ///< B�{�^��.
		GAMEPAD_BUTTON_X, ///< X�{�^��.
		GAMEPAD_BUTTON_Y, ///< Y�{�^��.
		GAMEPAD_BUTTON_L, ///< L�{�^��.
		GAMEPAD_BUTTON_R, ///< R�{�^��.
		GAMEPAD_BUTTON_BACK, ///< Back�{�^��.
		GAMEPAD_BUTTON_START, ///< Start�{�^��.
		GAMEPAD_BUTTON_L_THUMB, ///< ���X�e�B�b�N�{�^��.
		GAMEPAD_BUTTON_R_THUMB, ///< �E�X�e�B�b�N�{�^��.
		GAMEPAD_BUTTON_UP, ///< ��L�[.
		GAMEPAD_BUTTON_RIGHT, ///< �E�L�[.
		GAMEPAD_BUTTON_DOWN, ///< ���L�[.
		GAMEPAD_BUTTON_LEFT, ///< ���L�[.

	};

	/**
	 * �Q�[���p�b�h�̏�Ԃ��X�V����.
	 */
	void Window::UpdateGamePad()
	{
		// buttonDown�𐶐����邽�߂ɁA�X�V�O�̓��͏�Ԃ�ۑ����Ă���.
		const uint32_t prevButtons = gamepad.buttons;

		// �A�i���O���͂ƃ{�^�����͂��擾.
		int axesCount, buttonCount;
		const float* axes = glfwGetJoystickAxes(GLFW_JOYSTICK_1, &axesCount);		///(�Q�[���p�b�h�̐ڑ��ԍ�,�L���ȓ��̓f�[�^�����󂯎��ϐ��ւ̃|�C���^)
		const uint8_t* buttons = glfwGetJoystickButtons(GLFW_JOYSTICK_1, &buttonCount);

		// �����̔z��nullptr�łȂ��A�Œ���K�v�ȃf�[�^���𖞂����Ă���΁A�L���ȃQ�[���p�b�h��
		// �ڑ�����Ă���Ƃ݂Ȃ�.
		if (axes && buttons && axesCount >= 2 && buttonCount >= 8) {
			// �L���ȃQ�[���p�b�h���ڑ�����Ă���ꍇ

			// �����L�[�̓��͏�Ԃ��������āA���X�e�B�b�N�̓��͂Œu��������
			gamepad.buttons &=
				~(GamePad::DPAD_UP | GamePad::DPAD_DOWN | GamePad::DPAD_LEFT | GamePad::DPAD_RIGHT);
			static const float digitalThreshould = 0.3f;	// �f�W�^�����͂Ƃ݂Ȃ�臒l
			if (axes[GAMEPAD_AXES_LEFT_Y] >= digitalThreshould)
			{
				gamepad.buttons |= GamePad::DPAD_UP;
			}
			else if (axes[GAMEPAD_AXES_LEFT_Y] <= -digitalThreshould)
			{
				gamepad.buttons |= GamePad::DPAD_DOWN;
			}
			if (axes[GAMEPAD_AXES_LEFT_X] >= digitalThreshould)
			{
				gamepad.buttons |= GamePad::DPAD_LEFT;
			}
			else if (axes[GAMEPAD_AXES_LEFT_X] <= -digitalThreshould)
			{
				gamepad.buttons |= GamePad::DPAD_RIGHT;
			}
		}

		// �z��C���f�b�N�X��GamePad�L�[�̑Ή��\.
		static const struct 
		{
			int dataIndex;
			uint32_t gamepadBit;

		}keyMap[] = {
			{ GAMEPAD_BUTTON_A, GamePad::A },
			{ GAMEPAD_BUTTON_B, GamePad::B },
			{ GAMEPAD_BUTTON_X, GamePad::X },
			{ GAMEPAD_BUTTON_Y, GamePad::Y },
			{ GAMEPAD_BUTTON_L, GamePad::L },
			{ GAMEPAD_BUTTON_R, GamePad::R },
			{ GAMEPAD_BUTTON_START, GamePad::START },
			{ GAMEPAD_BUTTON_UP, GamePad::DPAD_UP },
			{ GAMEPAD_BUTTON_DOWN, GamePad::DPAD_DOWN },
			{ GAMEPAD_BUTTON_LEFT, GamePad::DPAD_LEFT },
			{ GAMEPAD_BUTTON_RIGHT, GamePad::DPAD_RIGHT },
		};

		for (const auto& e : keyMap) 
		{
			if (buttons[e.dataIndex] == GLFW_PRESS) {
				gamepad.buttons |= e.gamepadBit;
			}
			else if (buttons[e.dataIndex] == GLFW_RELEASE) {
				gamepad.buttons &= ~e.gamepadBit;
			}
			else
			{
				// �L���ȃQ�[���p�b�h���ڑ�����Ă��Ȃ��̂ŁA�L�[�{�[�h���͂ő�p

				// �z��C���f�b�N�X��GamePad�L�[�̑Ή��\
				static const struct
				{
					int keyCode;
					uint32_t gamepadBit;
				} keyMap[] =
				{
				{ GLFW_KEY_J, GamePad::A },
				{ GLFW_KEY_K, GamePad::B },
				{ GLFW_KEY_U, GamePad::X },
				{ GLFW_KEY_I, GamePad::Y },
				{ GLFW_KEY_O, GamePad::L },
				{ GLFW_KEY_L, GamePad::R },
				{ GLFW_KEY_ENTER, GamePad::START },
				{ GLFW_KEY_W, GamePad::DPAD_UP },
				{ GLFW_KEY_A, GamePad::DPAD_DOWN },
				{ GLFW_KEY_S, GamePad::DPAD_LEFT },
				{ GLFW_KEY_D, GamePad::DPAD_RIGHT },
				};

				for (const auto& e : keyMap)
				{
					const int key = glfwGetKey(window, e.keyCode);
					if (key == GLFW_PRESS)
					{
						gamepad.buttons |= e.gamepadBit;
					}
					else if (key == GLFW_RELEASE)
					{
						gamepad.buttons &= ~e.gamepadBit;
					}
				}
			}
		}

		// �O��̍X�V�ŉ�����Ȃ��āA���񉟂���Ă���L�[�̏���buttonDown�Ɋi�[.
		gamepad.buttonDown = gamepad.buttons & ~prevButtons;	/// 1,0��1,1��1��

	}

} /// namespace GLFWEW