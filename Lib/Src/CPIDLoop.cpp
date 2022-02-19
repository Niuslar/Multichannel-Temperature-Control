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

/**
 * @brief Internal control structure constructor.
 *
 * @param p_coeff Proportional coefficient.
 * @param i_coeff Integral coefficient.
 * @param d_coeff Differential coefficient.
 */
CPIDLoop::CONTROL_STRUCT_T::CONTROL_STRUCT_T(float p_coeff,
                                             float i_coeff,
                                             float d_coeff)
    : p_coeff(p_coeff),
      i_coeff(i_coeff),
      d_coeff(p_coeff),
      min_out(-std::numeric_limits<float>::max()),
      max_out(std::numeric_limits<float>::max()),
      min_i(-std::numeric_limits<float>::max()),
      max_i(std::numeric_limits<float>::max()),
      min_d(-std::numeric_limits<float>::max()),
      max_d(std::numeric_limits<float>::max())
{
}

/**
 * @brief Construct a PID loop.
 *
 * @param p_coeff Proportional coefficient.
 * @param i_coeff Integral coefficient.
 * @param d_coeff Differential coefficient.
 */
CPIDLoop::CPIDLoop(float p_coeff, float i_coeff, float d_coeff)
    : m_control_struct(p_coeff, i_coeff, d_coeff),
      m_old_target(0),
      m_old_error(0),
      m_p(0),
      m_i(0),
      m_d(0)
{
}

/**
 * @brief Run one loop of the PID.
 *
 * @param target Target value of control variable.
 * @param actual Actual value of control variable.
 * @return Control output to bring control variable to target.
 */
float CPIDLoop::run(float target, float actual)
{
    float error = target - actual;
    float output = run(target, actual, error - m_old_error);
    m_old_error = error;
    return output;
}

/**
 * @brief Run one loop of the PID.
 *
 * @param target Target value of control variable.
 * @param actual Actual value of control variable.
 * @param delta_error Error change rate to use in D term. Normally used for
 * noisy signals that require pre-filtering.
 * @return Control output to bring control variable to target.
 */
float CPIDLoop::run(float target, float actual, float delta_error)
{
    float error = target - actual;
    float output = 0;

    if (!(m_control_struct.p_coeff == 0 && m_control_struct.i_coeff == 0 &&
          m_control_struct.d_coeff == 0))
    {
        /**
         * @note Proportional term does not have limits. Overall output limits
         * apply to proportional term.
         */
        m_p = error * m_control_struct.p_coeff;

        /**
         * @note Integral term has "memory", i.e. error is accumulated over
         * time. As such care should be taken to ensure integral accumulator
         * does not end up with silly values, which then take time to unwind.
         * Various checks are performed during calculations to prevent this from
         * happening.
         */
        float old_i = m_i;
        m_i += error * m_control_struct.i_coeff;
        applyLimits(m_i, m_control_struct.min_i, m_control_struct.max_i);

        /**
         * @note Differential term works against rapid change, i.e. it tries to
         * respond to changes of error, not error itself. This property of D
         * term can cause severe instability if D coefficient is chosen poorly.
         * Be careful with D coefficient.
         */
        m_d = delta_error * m_control_struct.d_coeff;
        applyLimits(m_d, m_control_struct.min_d, m_control_struct.max_d);

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

/**
 * @brief Reset loop to constructed state. Erases all internal states.
 */
void CPIDLoop::reset()
{
    m_old_target = 0;
    m_old_error = 0;
    m_i = 0;
}

/**
 * @brief Limit variable value to min/max limits.
 *
 * @param input Variable to be limited.
 * @param min_limit Minimum allowed value.
 * @param max_limit Maximum allowed value.
 * @return Range limited value of the input argument.
 */
float CPIDLoop::applyLimits(float input, float min_limit, float max_limit) const
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
