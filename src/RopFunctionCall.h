#ifndef _ROP_FUNCTION_CALL_H_
#define _ROP_FUNCTION_CALL_H_

class RopFunctionCall
{
    public:
        RopFunctionCall(void);
        
        void setMap(GadgetMapPtr map);
        void setFunction(Function func);
        void setParameters(DataRefPtrList refs);
        
    private:
        GadgetMapPtr m_map;
        Function m_func;
        DataRefPtrList m_refs;
};

#endif _ROP_FUNCTION_CALL_H_
