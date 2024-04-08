#pragma once
#include "Drawable/Model.h"

namespace BBE 
{
	class GameObject 
	{
	public:
		GameObject(Model* a_Model);
		~GameObject();

		virtual void Update(Graphics& a_Graphics);

	private:
		Model* m_Model;
	};
}