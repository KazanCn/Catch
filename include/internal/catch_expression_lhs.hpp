/*
 *  Created by Phil on 11/5/2012.
 *  Copyright 2012 Two Blue Cubes Ltd. All rights reserved.
 *
 *  Distributed under the Boost Software License, Version 1.0. (See accompanying
 *  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
 */
#ifndef TWOBLUECUBES_CATCH_EXPRESSION_LHS_HPP_INCLUDED
#define TWOBLUECUBES_CATCH_EXPRESSION_LHS_HPP_INCLUDED

#include "catch_expressionresult_builder.h"
#include "catch_evaluate.hpp"

namespace Catch {

    struct STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison;

    template<typename T>
    inline void setResultIfBoolean( ExpressionResultBuilder&, const T& ) {}
    inline void setResultIfBoolean( ExpressionResultBuilder& result, bool value ) {
        result.setResultType( value );
    }

// Wraps the LHS of an expression and captures the operator and RHS (if any) - wrapping them all
// in an ExpressionResultBuilder object
template<typename T>
class ExpressionLhs {
	void operator = ( const ExpressionLhs& );

public:
    ExpressionLhs( T lhs ) : m_lhs( lhs ) {
        setResultIfBoolean( m_result.setLhs( Catch::toString( lhs ) ), lhs );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator == ( const RhsT& rhs ) {
        return captureExpression<Internal::IsEqualTo>( rhs );
    }

    template<typename RhsT>
    ExpressionResultBuilder& operator != ( const RhsT& rhs ) {
        return captureExpression<Internal::IsNotEqualTo>( rhs );
    }
    
    template<typename RhsT>
    ExpressionResultBuilder& operator < ( const RhsT& rhs ) {
        return captureExpression<Internal::IsLessThan>( rhs );
    }
    
    template<typename RhsT>
    ExpressionResultBuilder& operator > ( const RhsT& rhs ) {
        return captureExpression<Internal::IsGreaterThan>( rhs );
    }
    
    template<typename RhsT>
    ExpressionResultBuilder& operator <= ( const RhsT& rhs ) {
        return captureExpression<Internal::IsLessThanOrEqualTo>( rhs );
    }
    
    template<typename RhsT>
    ExpressionResultBuilder& operator >= ( const RhsT& rhs ) {
        return captureExpression<Internal::IsGreaterThanOrEqualTo>( rhs );
    }

    ExpressionResultBuilder& operator == ( bool rhs ) {
        return captureExpression<Internal::IsEqualTo>( rhs );
    }
    
    ExpressionResultBuilder& operator != ( bool rhs ) {
        return captureExpression<Internal::IsNotEqualTo>( rhs );
    }
    
    ExpressionResultBuilder& negate( bool shouldNegate ) {
        return m_result.negate( shouldNegate );
    }

    // Only simple binary expressions are allowed on the LHS.
    // If more complex compositions are required then place the sub expression in parentheses
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator + ( const RhsT& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator - ( const RhsT& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator / ( const RhsT& );
    template<typename RhsT> STATIC_ASSERT_Expression_Too_Complex_Please_Rewrite_As_Binary_Comparison& operator * ( const RhsT& );

private:
    template<Internal::Operator Op, typename RhsT>
    ExpressionResultBuilder& captureExpression( const RhsT& rhs ) {
        return m_result
            .setResultType( Internal::compare<Op>( m_lhs, rhs ) )
            .setRhs( Catch::toString( rhs ) )
            .setOp( Internal::OperatorTraits<Op>::getName() );
    }

private:
    ExpressionResultBuilder m_result;
    T m_lhs;
};

} // end namespace Catch

#endif // TWOBLUECUBES_CATCH_EXPRESSION_LHS_HPP_INCLUDED