
/*
	Interface for all eatable components such as cherries and dots
*/
class Eatable
{
public:
	virtual	void onEat(void* data) = 0;
	
	virtual float getScore() {
		return _score;
	}

	virtual void setScore(float score) {
		_score = score;
	}

protected:
	float _score;
};