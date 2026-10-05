#pragma once

class ProcessBase
{
protected:

public:
	/// <summary>
	/// ‰Šú‰»ˆ—
	/// </summary>
	virtual void Init() = 0;

	/// <summary>
	/// “ü—Íˆ—
	/// </summary>
	virtual void Input() = 0;

	/// <summary>
	/// XVˆ—
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// •`‰æˆ—
	/// </summary>
	virtual void Render() = 0;

	/// <summary>
	/// ‰¹ºÄ¶ˆ—
	/// </summary>
	virtual void Sound_play() = 0;
};