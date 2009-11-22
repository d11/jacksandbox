#include <vector>

using namespace std;

class Spline {
private:
	typedef std::vector<Vector*> tNodes;
	tNodes mNodes;
public:
	Spline() {}
	~Spline(){
		// Delete all nodes
		tNodes::iterator iter;
		for (iter = mNodes.begin(); iter != mNodes.end(); ++iter) {
			delete *iter;
		}
	}

	void AddNode(const Vector &node){
		// TODO check [0..1] ?


		mNodes.push_back(new Vector(node));
	}
	double GetValue(double x) const{
		// search for relevant nodes..
		// TODO binarysearch
		tNodes::const_iterator iter = mNodes.begin();
		Vector *v1 = *iter; ++iter;
		Vector *v2 = NULL;
		while (iter != mNodes.end()){
			v2 = *iter;
			if (v2->GetX() >= x) break; 
			++iter;
		}

		if (!v2) return 0.0; // throw

		// step
		return v1->GetY();

		// TODO linear

		// TODO cubic / cos?
	}
};