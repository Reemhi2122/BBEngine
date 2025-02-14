#include <vector>

class Graphics;
class GameObject;

namespace BBE
{
	namespace UI
	{
		void InitializeUI(Graphics& a_Graphics, std::vector<GameObject*>* a_GameObjectListPointer);
		void UpdateUI(Graphics& a_Graphics);
	}
}
