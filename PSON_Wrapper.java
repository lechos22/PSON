import java.util.ArrayList;
import java.util.HashMap;

class PSON_Wrapper {
    static{
        System.loadLibrary("PSONJNI");
    }
    public Object value;
    public PSON_Wrapper(SWIGTYPE_p_void obj){
        switch(PSON.get_type(obj)){
            case 'i': {
                Integer result = PSON.get_int(obj);
                value = result;
                break;
            }
            case 'f': {
                Float result = PSON.get_float(obj);
                value = result;
                break;
            }
            case 'S':
                value = PSON.get_str(obj);
                break;
            case 'A': {
                var arr = new ArrayList<Object>();
                for(long i = 0; i < PSON.get_len(obj); i++){
                    arr.add(new PSON_Wrapper(PSON.get_at(obj, i)).value);
                }
                value = arr;
                break;
            }
            case 'M':{
                var map = new HashMap<String, Object>();
                String idx = "";
                var i = PSON.get_map_iter(obj);
                for(; PSON.get_map_iter_alive(obj, i); idx = PSON.get_map_next(i)){
                    map.put(idx, new PSON_Wrapper(PSON.get_map_at(obj, idx)).value);
                }
                PSON.destroy_iter(i);
                value = map;
                break;
            }
            case '0':
                value = null;
                break;
        }
        PSON.destroy(obj);
    }
    public static Object parse(String str){
        return new PSON_Wrapper(PSON.parse(str)).value;
    }
}
