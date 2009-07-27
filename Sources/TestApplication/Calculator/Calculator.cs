using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AsContracts;

namespace TestApplication.Calculator
{
    class Calculator
    {
        // [AsContract("divisor != 0", "", "")]
        [AsContract(PreCondition="", Invariant="", PostCondition="")]
        public int Div(int divided, int divisor)
        {
            Test(divided);
            Calculator c = new Calculator();
            return divided / divisor;
        }

        public void Test(object arg)
        {
            object local = arg;
        }
    }
}
