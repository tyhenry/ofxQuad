#include "ofxQuad.h""

// 3d mesh vertices interpolator
void ofxQuad::interpolateMesh(int rows, int cols) {
	if (rows < 1 || cols < 1) return;

	_rows = rows; _cols = cols;

	_mesh.clearVertices();
	_mesh.getVertices().reserve(rows*cols + rows + cols + 1);

	ofPoint& tl = points[0]; // top left
	ofPoint& tr = points[1]; // top right
	ofPoint& bl = points[2]; // bottom left
	ofPoint& br = points[3]; // bottom right

	ofLogVerbose("ofxQuad") <<
		"interpolating points:\ntl("
		<< tl << "), tr(" << tr << ")\nbl("
		<< bl << "), br(" << br << ")\n"
		<< "into rows, cols: " << rows << ", " << cols;

	// top to bottom
	for (int r = 0; r <= rows; r++) {
		float pct = float(r) / rows;
		ofVec3f left(tl.getInterpolated(bl, pct));
		ofVec3f right(tr.getInterpolated(br, pct));

		// left to right
		for (int c = 0; c <= cols; c++) {
			float p = float(c) / cols;
			ofPoint v = left.getInterpolated(right, p);
			_mesh.getVertices().push_back(v);
			ofLogVerbose("ofxQuad") << "added point: (" << v << ")";
		}
	}
	ofLogVerbose("ofxQuad") << "created " << _mesh.getVertices().size() << " vertices";
}


// mesh triangulator
void ofxQuad::triangulateMesh(int rows, int cols)
{
	if (_mesh.getVertices().size() < 3) {
		ofLogError("ofxQuadMesh") << "empty mesh, can not triangulate";
		return;
	}

	_mesh.clearIndices();
	_mesh.setMode(OF_PRIMITIVE_TRIANGLES);
	for (int r = 0; r < rows; r++) {
		for (int c = 0; c < cols; c++) {
			int idx = c + r * (cols + 1); // since vertices are made cols+1

			_mesh.addIndex(idx);
			_mesh.addIndex(idx + 1);
			_mesh.addIndex(idx + cols + 1);

			_mesh.addIndex(idx + 1 + cols + 1);
			_mesh.addIndex(idx + 1);
			_mesh.addIndex(idx + cols + 1);
		}
	}
}

// 2d tex coords interpolator
void ofxQuad::assignTexCoords(float tw, float th) {
	if (tw < 1 || th < 1) return;

	ofPoint tl(0,0); // top left
	ofPoint tr(tw, 0); // top right
	ofPoint bl(0, th); // bottom left
	ofPoint br(tw, th); // bottom right

	_mesh.clearTexCoords();
	_mesh.getTexCoords().reserve(_rows*_cols);
	for (int r = 0; r <= _rows; r++) {
		float pct = float(r) / _rows;
		ofVec3f left(tl.getInterpolated(bl, pct));
		ofVec3f right(tr.getInterpolated(br, pct));

		for (int c = 0; c <= _cols; c++) {
			pct = float(c) / _cols;
			_mesh.getTexCoords().push_back(left.getInterpolated(right, pct));
		}
	}
}