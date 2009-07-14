// dllmain.h : Declaration of module class.

class CAsProfiledModule : public CAtlDllModuleT< CAsProfiledModule >
{
public :
	DECLARE_LIBID(LIBID_AsProfiledLib)
	DECLARE_REGISTRY_APPID_RESOURCEID(IDR_ASPROFILED, "{A0289EDB-6AE5-4717-B7A5-16C41173D1ED}")
};

extern class CAsProfiledModule _AtlModule;
