using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace AsContracts.ExpressionParser
{
    class MalformedExpressionException : Exception
    {
        public MalformedExpressionException() : base() { }
        public MalformedExpressionException(string msg) : base(msg) { }
    }
}
