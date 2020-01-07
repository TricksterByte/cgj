#pragma once
#ifndef __I_SCENE_NODE_CALLBACK_H__
#define __I_SCENE_NODE_CALLBACK_H__

class SceneNode;

class ISceneNodeCallback {
public:
	virtual void beforeDraw(SceneNode*) = 0;
	virtual void afterDraw(SceneNode*) = 0;
};

#endif