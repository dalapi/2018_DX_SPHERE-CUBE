#ifndef _OBJECT_H_
#define _OBJECT_H_

#define MAX_OBJECT (200)

class Object
{
public:
	Object();
	virtual ~Object();

	static void UninitObject(void);
	static void UpdateObject(void);
	static void DrawObject(void);

	// �������
	void Destroy(void) { this->destroy = true; }

protected:
	virtual void Uninit(void) {};
	virtual void Update(void) {};
	virtual void Draw(void) {};

private:
	static Object* index[MAX_OBJECT]; // ���̎����Ă鏑����

	static void DestroyCheck(void);

	int idx;
	bool destroy = false;
};

#endif