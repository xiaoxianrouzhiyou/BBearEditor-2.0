#ifndef BBUTILS_H
#define BBUTILS_H



#define BB_SAFE_DELETE(p) do{ delete p; p = NULL;} while(0)
#define BB_SAFE_DELETE_ARRAY(p) do{ delete []p; p = NULL;} while(0)
#define BB_PROCESS_ERROR(p) if(!p) break
#define BB_SUCCEEDED(hr) ((HRESULT)(hr)>=0)



class BBUtils
{
public:
    BBUtils();
    ~BBUtils();
};

#endif // BBUTILS_H
