#pragma once


enum class InputID
{
	Up,		//��ړ�
	Down,	//���ړ�
	Left,	//���ړ�
	Right,	//�E�ړ�
	Menu,	//���j���[���J��
	Decide,//����
	Cancel,//�L�����Z��
	BtnS,//S
	BtnN,//N
	Reset,//���Z�b�g�{�^��	*�L�[�R���t�B�O�Ŏg���Ă�
	Change,	//�{�^���P
	Lock,	//�{�^���Q
	Max
};




static InputID begin(InputID) { return InputID::Up; };
static InputID end(InputID) { return InputID::Max; };
static InputID operator++(InputID& state) {
	return (state = static_cast<InputID>(std::underlying_type<InputID>::type(state) + 1)); 
};
static InputID operator*(const InputID& state) { return state;};
