import java.util.ArrayList;
import java.util.HashMap;

class PSON_Wrapper {
    static{
        System.loadLibrary("PSONJNI");
    }
    private SWIGTYPE_p_void obj;
    public Object get_value(){
        switch(PSON.get_type(obj)){
            case 'i':{
                Integer result = PSON.get_int(obj);
                return result;
            }
            case 'f':{
                Float result = PSON.get_float(obj);
                return result;
            }
            case 'S':
                return PSON.get_str(obj);
            case 'A':{
                var arr = new ArrayList<Object>();
                for(long i = 0; i < PSON.get_len(obj); i++){
                    arr.add(new PSON_Wrapper(PSON.get_at(obj, i)).get_value());
                }
                return arr;
            }
            case 'M':{
                var map = new HashMap<String, Object>();
                String idx = "";
                for(var i = PSON.get_map_iter(obj); PSON.get_map_iter_alive(obj, i); idx = PSON.get_map_next(i)){
                    map.put(idx, new PSON_Wrapper(PSON.get_map_at(obj, idx)).get_value());
                }
                return map;
            }
            case '0':
                return null;
        }
        return new Object();
    }
    public PSON_Wrapper(SWIGTYPE_p_void obj){
        this.obj = obj;
    }
    public static PSON_Wrapper parse(String str){
        return new PSON_Wrapper(PSON.parse(str));
    }
}
