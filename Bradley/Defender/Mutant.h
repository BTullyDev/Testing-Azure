#pragma once
class Mutant : public EnemyBaseClass
{
public:
	Mutant(Point2f pos);
	~Mutant();

	void Update() override;
	void Draw() override;

	static void CreateMutant(Point2f pos);
protected:
private:
};

