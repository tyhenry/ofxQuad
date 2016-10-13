#pragma once
#include "ofMain.h"

class ofxQuad {
public:
	ofxQuad() {}
	ofxQuad(ofPoint tl, ofPoint tr, ofPoint bl, ofPoint br,
		int rows = 0, int cols = 0, float tw = 0, float th = 0, ofPrimitiveMode meshMode = OF_PRIMITIVE_POINTS)
	{
		set(tl, tr, bl, br);
		interpolateMesh(rows, cols);
		if (meshMode == OF_PRIMITIVE_TRIANGLES)
			triangulateMesh(rows, cols);
		assignTexCoords(tw, th);
	}

	ofPoint points[4];
	ofPoint& operator[](int i) {
		if (i >= 0 && i < 4) return points[i];
		return ofPoint();
	}
	void set(ofPoint tl, ofPoint tr, ofPoint bl, ofPoint br) {
		points[0] = tl; // top left
		points[1] = tr; // top right
		points[2] = bl; // bottom left
		points[3] = br; // bottom right
	}
	void interpolateMesh(int rows, int cols); // builds mesh vertices
	void triangulateMesh(int rows, int cols); // builds triangles
	void assignTexCoords(float tw, float th); // creates texture coords
	void assignTexCoords(ofTexture* tex) {
		if (tex != nullptr && tex->isAllocated())
			assignTexCoords(tex->getWidth(), tex->getHeight());
	}

	ofMesh& getMesh() { return _mesh; }

protected:
	ofMesh _mesh;
	int _rows, _cols = 0; // mesh interpolation factors
	float _tw, _th = 0; // texture dims
};