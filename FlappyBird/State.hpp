#pragma once
//trang thai
namespace Sonar
{
	class State
	{
	public:
		virtual void Init() = 0;//dung de khoi tao khi trang thai duoc load

		virtual void HandleInput() = 0;//xu li phim chuot 
		virtual void Update(float dt) = 0; //capnhatlogic game theo thoi gian
		virtual void Draw(float dt) = 0; // ve len man hinh

		virtual void Pause() { } // tam dung trang thai
		virtual void Resume() { }// tiep tuc trang thai sau khi tam dung
	};
}