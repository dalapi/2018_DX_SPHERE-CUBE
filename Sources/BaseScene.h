#ifndef _BASE_SCENE_H_
#define _BASE_SCENE_H_

class BaseScene
{
public:
	BaseScene();
	virtual ~BaseScene() = 0;

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

};

#endif _BASE_SCENE_H_