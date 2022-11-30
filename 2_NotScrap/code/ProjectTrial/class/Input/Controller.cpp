#include<DxLib.h>
#include "Controller.h"
#include "../../_debug/_DebugDispOut.h"


Controller::RingBuf* Controller::RingBuf::Create(int no)//�Ԃ�l�̏ꍇ�͖��O��Ԃ��w�肵�Ă�����
{
	int count = 0;
	auto beginBuf = new RingBuf{count,nullptr,nullptr};
	auto beforBuf = beginBuf;
	auto nowBuf = beginBuf;
	for (int j = 1; j < no; j++)
	{
		count++;
		nowBuf = new RingBuf{ count,beforBuf,nullptr };
		beforBuf->next = nowBuf;
		beforBuf = nowBuf;
	}
	beginBuf->befor = nowBuf;
	nowBuf->next = beginBuf;

	return beginBuf;

}

void Controller::RingBuf::Destroy(RingBuf* buf)//�����̏ꍇ�̓N���X����Ƃ��
{
	if (buf->befor == nullptr)
	{
		return;
	}
	buf->befor->next = nullptr;	//��O���猻�݂ւ̃|�C���^��f���؂�
	while (buf->next != nullptr)	//����null����Ȃ��Ȃ猻�݂�delete�B����null�Ȃ甲����
	{
		buf = buf->next;
		delete buf->befor;
	}
	delete buf;//���݂�delete
}

void Controller::WriteCmd(void)
{
	/*int writeData = 0;
	if (writeData)
	{

		if(ringBuf_->value() != writeData)
	}*/

	bool flag = false;
	for (auto id : InputID())
	{
		if (cntData_[id][static_cast<int>(Trg::Now)])
		{
			ringBuf_ = ringBuf_->next;
			ringBuf_->value = static_cast<int>(id);
			flag = true;
		}
	}
	if (!flag)
	{
		startBuf_ = ringBuf_;
	}
	else
	{
		if (startBuf_ == ringBuf_)//�X�^�[�g�ʒu�����炷
		{
			startBuf_ = startBuf_->next;
		}
	}

	auto drawBuf = startBuf_;
	int no = 0;
	while (drawBuf != ringBuf_)
	{
		_dbgDrawFormatString(no * 32, 560, 0xffffff, "%d", drawBuf->value);
		drawBuf = drawBuf->next;
		no++;
	}
}
