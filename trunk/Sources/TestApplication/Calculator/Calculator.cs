using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AsContracts;

namespace TestApplication.Calculator
{
    // [AsContract("a","b", "c")]
    class Calculator : Object
    {
        [CLSCompliant(true)]
        [AsContract(PreCondition="", Invariant="", PostCondition="")]
        public int Div(int divided, int divisor)
        {
            Test(this);
            return divided / divisor;
        }

        
        public void Test(Calculator arg)
        {
            object local = arg;
        }
    }
}
