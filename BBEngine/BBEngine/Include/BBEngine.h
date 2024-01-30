#pragma once
#include "BBRenderer.h"
#include "BBWindow.h"

#include "Utility/BBTimer.h"
#include "Utility/BBMemory.h"

#include "Drawable/Box.h"
#include "Drawable/Model.h"

namespace BBE {
	
	class BBEngine {
	public:
		BBEngine();
		~BBEngine();

		int StartBBEngine();
		void TestCode();

	private:
		void Update();
		void CheckInput();

	private:
		BBWindow m_Window;
		BBTimer m_Timer;
		BBWinRenderer m_BBWinRenderer;

		std::vector<Box*> m_Boxes;
		std::vector<Model*> m_Model;


		Allocators::ArenaAllocator m_ArenaAllocator;
		Allocators::StackAllocator m_StackAllocator;
		class ThreadPool* m_ThreadPool;
	};

}