#pragma once
#include "Base.h"
class object : public ProcessBase
{
public:

	//	‰Šú‰»ˆ—
	void Init()override;

	//@“ü—Íˆ—
	void Input()override;

	//	XVˆ—
	void Update()override;

	//	•`‰æˆ—
	void Render()override;

	//	‰¹ºÄ¶ˆ—
	void Sound_play()override;
};

