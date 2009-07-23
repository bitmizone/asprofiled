using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using AsContracts.ExpressionParser;

namespace AsContracts
{

    public class AsContractAttribute : Attribute
    {
        private string _preCondition;
        private string _invariant;
        private string _postCondition;

        public AsContractAttribute(string preCondition, string invariant, string postCondition)
        {

        }

        public string PostCondition
        {
            get { return _postCondition; }
            set
            {
                Parser.ParseExpression(value);
                _postCondition = value;
            }
        }

        public string Invariant
        {
            get { return _invariant; }
            set
            {
                Parser.ParseExpression(value);
                _invariant = value;
            }
        }

        public string PreCondition
        {
            get { return _preCondition; }
            set
            {
                Parser.ParseExpression(value);
                _preCondition = value;
            }
        }
    }
}
