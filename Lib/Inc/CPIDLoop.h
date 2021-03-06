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

#include <limits>

/* Default PID parameters.
 * These are setup to make PID loop transparent. Output will be equal to error.
 * Proportional is percentage of power per unit of error.
 * Integral is units of output per unit of error per unit of time.
 * Differential is unit of output per degree error change per unit of time.
 * Normally for slow moving processes differential is zero.
 */
#define DEFAULT_P_COEFF 1
#define DEFAULT_I_COEFF 0
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

    enum LIMITS_T
    {
        ELimits_Int,
        ELimits_Diff,
        ELimits_Out
    };

    CPIDLoop(float k_p = DEFAULT_P_COEFF,
             float k_i = DEFAULT_I_COEFF,
             float k_d = DEFAULT_D_COEFF);

    float run(float target, float actual);
    float run(float target, float actual, float deltaError);
    void reset();

    /* PID runtime terms access methods. */
    /**
     * @brief Get proportional contribution.
     *
     * @return Proportional contribution to PID output.
     */
    float getP() const
    {
        return m_p;
    }

    /**
     * @brief Get integral contribution.
     *
     * @return Integral contribution to PID output.
     */
    float getI() const
    {
        return m_i;
    }

    /**
     * @brief Get differential contribution.
     *
     * @return Differential contribution to PID output.
     */
    float getD() const
    {
        return m_d;
    }

    /**
     * @brief Set integral contribution. This value is added to integral
     * accumulator and acts as output preload.
     *
     * @param i Integral contribution value.
     */

    void setI(float i)
    {
        m_i = i;
    }

    bool setLimits(LIMITS_T limit_type,
                   float min = -std::numeric_limits<float>::max(),
                   float max = std::numeric_limits<float>::max());

    /**
     * @brief Get current control structure.
     *
     * @return Current control structure. Passed by reference.
     */
    const CONTROL_STRUCT_T &getControlStruct() const
    {
        return m_control_struct;
    }

private:
    float applyLimits(float input, float min_limit, float max_limit) const;

    CONTROL_STRUCT_T m_control_struct;

    float m_old_target;
    float m_old_error;

    float m_p;
    float m_i;
    float m_d;
};

#endif /* CPIDLOOP_H_ */
