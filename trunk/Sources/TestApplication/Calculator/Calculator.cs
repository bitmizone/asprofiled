using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;

namespace TestApplication.Calculator
{
    // [AsContract("a","b", "c")]
    class Calculator : Object
    {
        //[AsContract(PreCondition="bieda", Invariant="", PostCondition="")] // named args
        [AsContract(null, "biedactwo > 4", null)] // fixed args
        public int Div(bool t, int divided, int divisor, string val)
        {
            Test(this);
            string te = val;
            if (divisor == 0)
            {
                return 0;
            }
            return divided / divisor;
        }

        
        public void Test(Calculator arg)
        {
            object local = arg;
        }


    }

    [AttributeUsage(AttributeTargets.All, AllowMultiple=true)]
    public class AsContractAttribute : Attribute
    {

        public AsContractAttribute()
        {
        }

        public AsContractAttribute(string preCondition, string invariant, string postCondition)
        {
            
        }

        public string PostCondition
        {
            get;
            set;
        }

        public string Invariant
        {
            get;
            set;
        }

        public string PreCondition
        {
            get;
            set;

        }
    }
}
