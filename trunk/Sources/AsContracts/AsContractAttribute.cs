using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;


namespace AsContracts
{

    public class AsContractAttribute : Attribute
    {

        public AsContractAttribute()
        {
        }

        public AsContractAttribute(string preCondition, string invariant, string postCondition)
        {
            Console.WriteLine("Contract constructed");
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
