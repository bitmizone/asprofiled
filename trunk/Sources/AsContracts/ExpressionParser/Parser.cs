using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace AsContracts.ExpressionParser
{
    class Parser
    {
        private Parser() { }

        public static void ParseExpression(string expression)
        {
            if (expression == null) {
                throw new MalformedExpressionException();
            }
        }
    }
}
