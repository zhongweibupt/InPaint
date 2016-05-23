#include "CEdge.h"
#include "CMatcher.h"

int main()
{
	char src[] = "src.jpg";
	char model[] = "model.jpg";
	char dst[] = "dst.jpg";

	float edgeThresh = 120;
	float radius = 10;
	int lo = 80;
	int hi = 254;

	CMatcher matcher;
	matcher.setSrc(src);
	matcher.setParameter(edgeThresh, radius, lo, hi);
	matcher.match(model);
	matcher.write(src);

	return 1;
}