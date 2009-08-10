using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AsContracts;

namespace TestApplication.Calculator
{
    // [AsContract("a","b", "c")]
    class Calculator
    {
        // [AsContract("divisor != 0", "", "")]
        [AsContract(PreCondition="", Invariant="", PostCondition="")]
        public int Div(int divided, int divisor)
        {
            Test(divided);
            return divided / divisor;
        }

        public void Test(object arg)
        {
            object local = arg;
        }
    }
}
