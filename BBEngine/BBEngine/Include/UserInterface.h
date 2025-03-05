#include <vector>

class Graphics;
class BBObject;

namespace BBE
{
	namespace UI
	{
		void InitializeUI(Graphics& a_Graphics, std::vector<BBObject*>* a_GameObjectListPointer);
		void UpdateUI(Graphics& a_Graphics);
	}
}
