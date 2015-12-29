# To solve #

  * Is it valid when it comes to reading value of object in FunctionEnter
```
ObjectID* id = reinterpret_cast<ObjectID*>( range.startAddress);
```
  * attribute's metadata is not visible when containing assembly hasn't been loaded.
  * read and parse attribute's constructor metadata

# Recently solved #

  * get metadata token of constructor (method) of the attribute attached to contracted method.


**Solution**: Get customAttributeProps with second parameter as an owner of an attribute. I.E.` pMetaDataImport->GetCustomAttributeProps(*currentAttribute, &methodToken, &attributeConstructor, NULL, NULL); ` After this call attributeConstructor will be handler to constructor that we are interested in.

---

  * Events of entering and leaving function are not correctly captured and handled which result in ending program.


**Solution**: Register EnterLeaveFunctionHooks2 as follows:
```
_ICorProfilerInfo2->SetEnterLeaveFunctionHooks2( &FunctionEnterHandler, &FunctionLeaveHandler, NULL);
```

Definitions of those handlers :
```
void _declspec(naked) FunctionLeaveHandler(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_RANGE *retvalRange)
{
	__asm 
	{
		push	ebp
		mov		ebp,esp
		pushad
		mov		eax, [ebp+0x14]
		push	eax
		mov		eax, [ebp+0x10]
		push	eax
		mov		eax, [ebp+0x0C]
		push	eax
		mov		eax, [ebp+0x08]
		push	eax
		call	FunctionLeaveGlobal
		popad
		pop		ebp
		ret		16
	}
}


void _declspec(naked) FunctionEnterHandler(FunctionID functionID, UINT_PTR clientData, COR_PRF_FRAME_INFO func, COR_PRF_FUNCTION_ARGUMENT_INFO *argumentInfo) 
{
	__asm 
	{
		push	ebp // create stack frame
		mov		ebp, esp // move current stack pointer to ebp
		pushad
		mov		eax, [ebp+0x14]
		push	eax
		mov		eax, [ebp+0x10]
		push	eax
		mov		eax, [ebp+0x0C]
		push	eax
		mov		eax, [ebp+0x08]
		push	eax
		call	FunctionEnterGlobal
		popad
		pop		ebp
		ret		16
	}
}
```

---
