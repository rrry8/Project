#include "Curve.h"
#include <cmath>    //< for std::sqrt, std::cbrt, std::pow
#include <complex>  //< for std::complex
#include <iostream> //< for std::cout
/**
      * @brief �������t���̃R���X�g���N�^�D
      *
      * @param j_max   �ő���x�̑傫�� [m/s/s/s], ���ł��邱��
      * @param a_max   �ő�����x�̑傫�� [m/s/s], ���ł��邱��
      * @param v_start �n�_���x [m/s]
      * @param v_end   �I�_���x [m/s]
      */
Curve::Curve(const float j_max, const float a_max, const float v_start,const float v_end)
{
    reset(j_max, a_max, v_start, v_end);
}

Curve::Curve()
{
    jm = am = t0 = t1 = t2 = t3 = v0 = v1 = v2 = v3 = x0 = x1 = x2 = x3 = 0;
}

Curve::~Curve()
{
}
/**
        * @brief �����̍S����������Ȑ��𐶐�����D
        * ���̊֐��ɂ���āC���ׂĂ̕ϐ��������������D(�R��͂Ȃ�)
        *
        * @param j_max   �ő���x�̑傫�� [m/s/s/s], ���ł��邱��
        * @param a_max   �ő�����x�̑傫�� [m/s/s], ���ł��邱��
        * @param v_start �n�_���x [m/s]
        * @param v_end   �I�_���x [m/s]
        */
void Curve::reset(const float j_max, const float a_max, const float v_start, const float v_end)
{
    /* �����t���ő�� */
    am = (v_end > v_start) ? a_max : -a_max; //< �ő�����x�̕���������
    jm = (v_end > v_start) ? j_max : -j_max; //< �ő���x�̕���������
    /* �����l�ƍŏI�l���� */
    v0 = v_start; //< ���
    v3 = v_end;   //< ���
    t0 = 0;       //< �����ł͏����l���[���Ƃ���
    x0 = 0;       //< �����ł͏����l�̓[���Ƃ���
    /* ���x���Ȑ��ƂȂ镔���̎��Ԃ����� */
    const auto tc = a_max / j_max;
    /* �������x�����^���̎��Ԃ����� */
    const auto tm = (v3 - v0) / am - tc;
    /* �������x�����^���̗L���ŕ��� */
    if (tm > 0)
    {
        /* ���x: �Ȑ� -> ���� -> �Ȑ� */
        t1 = t0 + tc;
        t2 = t1 + tm;
        t3 = t2 + tc;
        v1 = v0 + am * tc / 2;                //< v(t) ��ϕ�
        v2 = v1 + am * tm;                    //< v(t) ��ϕ�
        x1 = x0 + v0 * tc + am * tc * tc / 6; //< x(t) ��ϕ�
        x2 = x1 + v1 * tm;                    //< x(t) ��ϕ�
        x3 = x0 + (v0 + v3) / 2 * (t3 - t0); //< v(t) �O���t�̑�`�̖ʐς��
    }
    else
    {
        /* ���x: �Ȑ� -> �Ȑ� */
        const auto tcp = std::sqrt((v3 - v0) / jm); //< �ϋȂ܂ł̎���
        t1 = t2 = t0 + tcp;
        t3 = t2 + tcp;
        v1 = v2 = (v0 + v3) / 2; //< �Ώ̐���蒆�_�ƂȂ�
        x1 = x2 = x0 + v1 * tcp + jm * tcp * tcp * tcp / 6; //< x(t) ��ϕ�
        x3 = x0 + 2 * v1 * tcp; //< ���x v(t) �O���t�̖ʐς��
    }
}
/**
         * @brief ���s��������B������I�_���x���Z�o����֐�
         *
         * @param j_max �ő���x�̑傫�� [m/s/s/s], ���ł��邱��
         * @param a_max �ő�����x�̑傫�� [m/s/s], ���ł��邱��
         * @param vs    �n�_���x [m/s]
         * @param vt    �ڕW���x [m/s]
         * @param d     ���s���� [m]
         * @return ve   �I�_���x [m/s]
         */
float Curve::calcVelocityEnd(const float j_max, const float a_max, const float vs, const float vt, const float d)
{
    /* ���x���Ȑ��ƂȂ镔���̎��Ԃ����� */
    const auto tc = a_max / j_max;
    /* �ő�����x�̕��������� */
    const auto am = (vt > vs) ? a_max : -a_max;
    const auto jm = (vt > vs) ? j_max : -j_max;
    /* �������x�����^���̗L���ŕ��� */
    const auto d_triangle = (vs + am * tc / 2) * tc; //< distance @ tm == 0
    const auto v_triangle = jm / am * d - vs;        //< v_end @ tm == 0
    // logd << "d_tri: " << d_triangle << std::endl;
    // logd << "v_tri: " << v_triangle << std::endl;
    if (d * v_triangle > 0 && std::abs(d) > std::abs(d_triangle))
    {
        /* �Ȑ��E�����E�Ȑ� */
        //logd << "v: curve - straight - curve" << std::endl;
        /* 2���������̉��̌��������� */
        const auto amtc = am * tc;
        const auto D = amtc * amtc - 4 * (amtc * vs - vs * vs - 2 * am * d);
        const auto sqrtD = std::sqrt(D);
        return (-amtc + (d > 0 ? sqrtD : -sqrtD)) / 2;
    }
    /* �Ȑ��E�Ȑ� (���s�������Z������) */
    /* 3���������������āC�I�_���x���Z�o;
     * �ȒP�̂��߁C�l����x���ׂĐ��ɕϊ����āC�v�Z���ʂɕ�����t�^���ĕԑ� */
    const auto a = std::abs(vs);
    const auto b = (d > 0 ? 1 : -1) * jm * d * d;
    const auto aaa = a * a * a;
    const auto c0 = 27 * (32 * aaa * b + 27 * b * b);
    const auto c1 = 16 * aaa + 27 * b;
    if (c0 >= 0)
    {
        /* ���[�g�̒����񕉂̂Ƃ� */
        //logd << "v: curve - curve (accel)" << std::endl;
        const auto c2 = std::cbrt((std::sqrt(c0) + c1) / 2);
        return (d > 0 ? 1 : -1) * (c2 + 4 * a * a / c2 - a) / 3; //< 3���������̉�
    }
    else
    {
        /* ���[�g�̒������̂Ƃ� */
        //logd << "v: curve - curve (decel)" << std::endl;
        const auto c2 = std::pow(std::complex<float>(c1 / 2, std::sqrt(-c0) / 2),
            float(1) / 3);
        return (d > 0 ? 1 : -1) * (c2.real() * 2 - a) / 3; //< 3���������̉�
    }
}
/**
         * @brief ���s��������B������ő呬�x���Z�o����֐�
         *
         * @param j_max �ő���x�̑傫�� [m/s/s/s], ���ł��邱��
         * @param a_max �ő�����x�̑傫�� [m/s/s], ���ł��邱��
         * @param vs    �n�_���x [m/s]
         * @param ve    �I�_���x [m/s]
         * @param d     ���s���� [m]
         * @return vm   �ő呬�x [m/s]
         */
float Curve::calcVelocityMax(const float j_max, const float a_max, const float vs, const float ve, const float d)
{
    /* ���x���Ȑ��ƂȂ镔���̎��Ԃ����� */
    const auto tc = a_max / j_max;
    const auto am = (d > 0) ? a_max : -a_max; /*< ���������͈ړ������Ɉˑ� */
    /* 2���������̉��̌��������� */
    const auto amtc = am * tc;
    const auto D = amtc * amtc - 2 * (vs + ve) * amtc + 4 * am * d +
        2 * (vs * vs + ve * ve);
    if (D < 0)
    {
        /* �S���������������� */
        //loge << "Error! D = " << D << " < 0" << std::endl;
        /* ���͂̃`�F�b�N */
        if (vs * ve < 0)
            //loge << "Invalid Input! vs: " << vs << ", ve: " << ve << std::endl;
            return vs;
    }
    const auto sqrtD = std::sqrt(D);
    return (-amtc + (d > 0 ? sqrtD : -sqrtD)) / 2; //< 2���������̉�
}
/**
        * @brief ���x������ψʂ��Z�o����֐�
        *
        * @param j_max   �ő���x�̑傫�� [m/s/s/s], ���ł��邱��
        * @param a_max   �ő�����x�̑傫�� [m/s/s], ���ł��邱��
        * @param v_start �n�_���x [m/s]
        * @param v_end   �I�_���x [m/s]
        * @return d      �ψ� [m]
        */
float Curve::calcMinDistance(const float j_max, const float a_max, const float v_start, const float v_end)
{
    /* �����t���ő�� */
    const auto am = (v_end > v_start) ? a_max : -a_max;
    const auto jm = (v_end > v_start) ? j_max : -j_max;
    /* ���x���Ȑ��ƂȂ镔���̎��Ԃ����� */
    const auto tc = a_max / j_max;
    /* �������x�����^���̎��Ԃ����� */
    const auto tm = (v_end - v_start) / am - tc;
    /* �n�_����I�_�܂ł̎��Ԃ����� */
    const auto t_all =
        (tm > 0) ? (tc + tm + tc) : (2 * std::sqrt((v_end - v_start) / jm));
    return (v_start + v_end) / 2 * t_all; //< ���x�O���t�̖ʐςɂ��
}
