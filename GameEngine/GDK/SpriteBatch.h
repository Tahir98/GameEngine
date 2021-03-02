#include "Program.h"
#include "VertexBuffer.h"
#include "VertexArray.h"
#include "IndexBuffer.h"
#include <vector>
#include <utility>
#include "Texture.h"

class SpriteBatch {
private:
	VertexBuffer vb;
	VertexArray va;
	Program program;

	std::vector<float> vertices;
	std::vector<std::pair<int,int>> ids;

	int maxTexSlots;
	int usedTexSlots = 0;
	bool isBegin = false;

public:
	SpriteBatch();
	~SpriteBatch();

	void begin();
	void end();

	void draw(Texture& texture, const float x,const float y,const float width,const float height);

private:
	inline void addVertices(const float x, const float y, const float width, const float height,const int slot);

};