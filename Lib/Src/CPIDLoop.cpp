/**
 * @file CPIDLoop.cpp
 */

/*
 * CPIDLoop.cpp
 *
 *  Created on: 19 Feb 2022
 *      Author: salavat.magazov
 */

#include "CPIDLoop.h"
#include <limits>

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

CPIDLoop::CONTROL_STRUCT_T::CONTROL_STRUCT_T(float k_p, float k_i, float k_d)
    : p_coeff(k_p),
      i_coeff(k_i),
      d_coeff(k_p),
      min_out(-std::numeric_limits<float>::max()),
      max_out(std::numeric_limits<float>::max()),
      min_i(-std::numeric_limits<float>::max()),
      max_i(std::numeric_limits<float>::max()),
      min_d(-std::numeric_limits<float>::max()),
      max_d(std::numeric_limits<float>::max())
{
}

CPIDLoop::CPIDLoop(float k_p, float k_i, float k_d)
    : m_control_struct(k_p, k_i, k_d),
      m_old_target(0),
      m_old_error(0),
      m_p(0),
      m_i(0),
      m_d(0)
{
}

float CPIDLoop::run(float target, float actual)
{
    float error = target - actual;
    float output = run(target, actual, error - m_old_error);
    m_old_error = error;
    return output;
}

float CPIDLoop::run(float target, float actual, float deltaError)
{
    float error = target - actual;
    float output = 0;

    if (!(m_control_struct.k_p == 0 && m_control_struct.k_i == 0 &&
          m_control_struct.k_d == 0))
    {
        /**
         * @note Proportional term does not have limits. Overall output limits
         * apply to proportional term.
         */
        m_p = error * m_control_struct.k_p;

        /**
         * @note Integral term has "memory", i.e. error is accumulated over
         * time. As such care should be taken to ensure integral accumulator
         * does not end up with silly values, which then take time to unwind.
         * Various checks are performed during calculations to prevent this from
         * happening.
         */
        float old_i = m_i;
        m_i += error * m_control_struct.k_i;
        applyLimits(m_i, m_control_struct.min_i, m_control_struct.max_i);
        /* Differential term
         * Dterm = ((target - actual) - oldError) * k_d
         */
        m_d = deltaError * m_control_struct.k_d;
        if (m_d > m_control_struct.max_d)
        {
            m_d = m_control_struct.max_d;
        }
        if (m_d < m_control_struct.min_d)
        {
            m_d = m_control_struct.min_d;
        }

        /* Calculate output and perform range checking.
         * Integral term control is implemented to prevent integral windup.
         */
        output = m_p + m_i + m_d;
        if (output > m_control_struct.max_out)
        {
            output = m_control_struct.max_out;
            /* integral wind-up prevention */
            if (m_i > old_i)
            {
                m_i = old_i;
            }
        }
        else if (output < m_control_struct.min_out)
        {
            output = m_control_struct.min_out;
            /* integral wind-up prevention */
            if (m_i < old_i)
            {
                m_i = old_i;
            }
        }
    }

    return output;
}

void CPIDLoop::reset() {}
void CPIDLoop::resetParam() {}

float CPIDLoop::applyLimits(float input, float min_limit, float max_limit)
{
    if (max_limit < input)
    {
        input = max_limit;
    }
    if (input < min_limit)
    {
        input = min_limit;
    }
    return input;
}
