#include "StdAfx.h"
#include "ParamParser.h"

CParamParser::CParamParser(IMetaDataImport& pMetaDataIdentifier) : pMetaData(pMetaDataIdentifier) {	}


CParamParser::~CParamParser(void)
{

}

PCCOR_SIGNATURE CParamParser::ParseSignature(PCCOR_SIGNATURE signature, LPWSTR signatureText)
{	
	COR_SIGNATURE corSignature = *signature++;

	switch (corSignature) 
	{	
	case ELEMENT_TYPE_VOID:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"void");
		break;							
	case ELEMENT_TYPE_BOOLEAN:	
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"bool");	
		break;			
	case ELEMENT_TYPE_CHAR:
		wcscat_s(signatureText, NAME_BUFFER_SIZE , L"wchar");	
		break;							
	case ELEMENT_TYPE_I1:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"int8");	
		break;		 		
	case ELEMENT_TYPE_U1:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"unsigned int8");	
		break;	 		
	case ELEMENT_TYPE_I2:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"int16");	
		break;		
	case ELEMENT_TYPE_U2:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"unsigned int16");	
		break;					
	case ELEMENT_TYPE_I4:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"int32");	
		break;        
	case ELEMENT_TYPE_U4:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"unsigned int32");	
		break;				
	case ELEMENT_TYPE_I8:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"int64");	
		break;				
	case ELEMENT_TYPE_U8:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"unsigned int64");	
		break;		
	case ELEMENT_TYPE_R4:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"float32");	
		break;					
	case ELEMENT_TYPE_R8:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"float64");	
		break;			 		
	case ELEMENT_TYPE_STRING:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"String");	
		break;	
	case ELEMENT_TYPE_VAR:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"class variable(unsigned int8)");	
		break;		
	case ELEMENT_TYPE_MVAR:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"method variable(unsigned int8)");	
		break;					         
	case ELEMENT_TYPE_TYPEDBYREF:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"refany");	
		break;		 		
	case ELEMENT_TYPE_I:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"int");	
		break;			
	case ELEMENT_TYPE_U:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"unsigned int");	
		break;			  		
	case ELEMENT_TYPE_OBJECT:
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"Object");	
		break;		       
	case ELEMENT_TYPE_SZARRAY:	 
		signature = ParseSignature(signature, signatureText); 
		wcscat_s(signatureText, NAME_BUFFER_SIZE,  L"[]");
		break;				
	case ELEMENT_TYPE_PINNED:
		signature = ParseSignature( signature, signatureText); 
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"pinned");	
		break;	        
	case ELEMENT_TYPE_PTR:   
		signature = ParseSignature(signature, signatureText); 
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"*");	
		break;           
	case ELEMENT_TYPE_BYREF:   
		signature = ParseSignature(signature, signatureText); 
		wcscat_s(signatureText, NAME_BUFFER_SIZE, L"&");	
		break;
	case ELEMENT_TYPE_VALUETYPE:   
	case ELEMENT_TYPE_CLASS:	
	case ELEMENT_TYPE_CMOD_REQD:
	case ELEMENT_TYPE_CMOD_OPT:
		{
			mdToken	token;	
			signature += CorSigUncompressToken( signature, &token ); 

			WCHAR className[ NAME_BUFFER_SIZE ];
			if ( TypeFromToken( token ) == mdtTypeRef )
			{
				
				pMetaData.GetTypeRefProps(token, NULL, className, NAME_BUFFER_SIZE, NULL);
			}
			else
			{
				pMetaData.GetTypeDefProps(token, className, NAME_BUFFER_SIZE, NULL, NULL, NULL );
			}

			wcscat_s(signatureText, NAME_BUFFER_SIZE, className );
		}
		break;		
	case ELEMENT_TYPE_GENERICINST:
		{
			signature = ParseSignature( signature, signatureText); 

			wcscat_s(signatureText, NAME_BUFFER_SIZE, L"<");	
			ULONG arguments = CorSigUncompressData(signature);
			for (ULONG i = 0; i < arguments; ++i)
			{
				if(i != 0)
				{
					wcscat_s(signatureText, NAME_BUFFER_SIZE, L", ");	
				}

				signature = ParseSignature( signature, signatureText);
			}
			wcscat_s(signatureText, NAME_BUFFER_SIZE, L">");	
		}
		break;		        
	case ELEMENT_TYPE_ARRAY:	
		{
			signature = ParseSignature(signature, signatureText);              
			ULONG rank = CorSigUncompressData(signature);													
			if ( rank == 0 ) 
			{
				wcscat_s(signatureText, NAME_BUFFER_SIZE, L"[?]");
			}
			else 
			{		
				ULONG arraysize = (sizeof(ULONG) * 2 * rank);
				ULONG *lower = (ULONG *)_alloca(arraysize);
				memset(lower, 0, arraysize); 
				ULONG *sizes = &lower[rank];

				ULONG numsizes = CorSigUncompressData(signature);	
				for (ULONG i = 0; i < numsizes && i < rank; i++)
				{
					sizes[i] = CorSigUncompressData(signature);	
				}

				ULONG numlower = CorSigUncompressData(signature);	
				for (ULONG i = 0; i < numlower && i < rank; i++)	
				{
					lower[i] = CorSigUncompressData( signature ); 
				}

				wcscat_s(signatureText, NAME_BUFFER_SIZE, L"[");	
				for (ULONG i = 0; i < rank; ++i)	
				{					
					if (i > 0) 
					{
						wcscat_s(signatureText, NAME_BUFFER_SIZE, L",");
					}

					if (lower[i] == 0)
					{
						if(sizes[i] != 0)
						{
							WCHAR *size = new WCHAR[NAME_BUFFER_SIZE];
							size[0] = '\0';
							wsprintf(size, L"%d", sizes[i]);											
							wcscat_s(signatureText, NAME_BUFFER_SIZE, size);
						}
					}	
					else	
					{
						WCHAR *low = new WCHAR[NAME_BUFFER_SIZE];
						low[0] = '\0';
						wsprintf(low, L"%d", lower[i]);
						wcscat_s(signatureText, NAME_BUFFER_SIZE, low);
						wcscat_s(signatureText, 3, L"...");	

						if (sizes[i] != 0)	
						{
							WCHAR *size = new WCHAR[NAME_BUFFER_SIZE];
							size[0] = '\0';
							wsprintf(size, L"%d", (lower[i] + sizes[i] + 1));
							wcscat_s(signatureText, NAME_BUFFER_SIZE,  size);
						}
					}
				}
				wcscat_s(signatureText, NAME_BUFFER_SIZE, L"]");
			}
		} 
		break;   		    
	default:	
	case ELEMENT_TYPE_END:	
	case ELEMENT_TYPE_SENTINEL:	
		WCHAR *elementType = new WCHAR[NAME_BUFFER_SIZE];
		elementType[0] = '\0';
		wsprintf(elementType, L"<UNKNOWN:0x%X>", corSignature);
		wcscat_s(signatureText, NAME_BUFFER_SIZE, elementType);
		break;				                      				            
	}

	return signature;
}