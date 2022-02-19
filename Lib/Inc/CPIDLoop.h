/**
 * @file CPIDLoop.h
 */

/*
 * CPIDLoop.h
 *
 *  Created on: 19 Feb 2022
 *      Author: salavat.magazov
 */

#ifndef CPIDLOOP_H_
#define CPIDLOOP_H_

/* Default PID parameters.
 * These should be close enough to make control fast, but stable.
 * Proportional is percentage of power per unit of error.
 * Integral is units of output per unit of error per minute.
 * Differential is unit of output per degree error change per minute.
 * Normally for slow moving processes differential is zero.
 */
#define DEFAULT_P_COEFF 1
#define DEFAULT_I_COEFF (1.0 / (60.0 * 60.0))
#define DEFAULT_D_COEFF 0

class CPIDLoop
{
public:
    struct CONTROL_STRUCT_T
    {
        CONTROL_STRUCT_T(float k_p, float k_i, float k_d);

        float p_coeff;
        float i_coeff;
        float d_coeff;

        float min_out;
        float max_out;

        float min_i;
        float max_i;

        float min_d;
        float max_d;
    };

    CPIDLoop(float k_p = DEFAULT_P_COEFF,
             float k_i = DEFAULT_I_COEFF,
             float k_d = DEFAULT_D_COEFF);

    float run(float target, float actual);
    float run(float target, float actual, float deltaError);
    void reset();

    /* PID runtime terms access methods. */
    float getP() const
    {
        return m_p;
    }

    float getI() const
    {
        return m_i;
    }
    float getD() const
    {
        return m_d;
    }
    void setI(float i)
    {
        m_i = i;
    }

    void setupLoop(const CONTROL_STRUCT_T &control_struct)
    {
        m_control_struct = control_struct;
    }

    const CONTROL_STRUCT_T &getControlStruct() const
    {
        return m_control_struct;
    }

private:
    float applyLimits(float input, float min_limit, float max_limit);

    CONTROL_STRUCT_T m_control_struct;

    float m_old_target;
    float m_old_error;

    float m_p;
    float m_i;
    float m_d;
};

#endif /* CPIDLOOP_H_ */
