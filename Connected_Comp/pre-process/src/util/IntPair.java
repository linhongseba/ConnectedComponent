/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

package util;

import java.util.HashMap;
import java.util.Iterator;
import java.util.Map;

/**
 *
 * @author linhong
 */
public class IntPair extends Pair implements Comparable {
    public IntPair(int index, int value){
         super(index,value);
    }
        public void setindex(int indexvalue){
        setKey(indexvalue);
    }
    public void setvalue(int inputvalue){
        setValue(inputvalue);
    }
    public int getsource(){
        return ((Integer)this.getKey()).intValue();
    }
    public int gettarget(){
        return ((Integer)this.getValue()).intValue();
    }
    public int compareTo(Object o) {
         return compareTo((IntPair)o);
    }
    public int compareTo(IntPair o) {
        if(o.getsource()==getsource()&&o.gettarget()==gettarget())
            return 0;
        else if(getsource()>o.getsource())
            return 1;
        else if(getsource()<o.getsource())
            return -1;
        else
            if(getsource()==o.getsource()&&gettarget()>o.gettarget())
                return 1;
            else return -1;   
    }
    public boolean equals(Object o){
        if(this == o) 
            return true;
        if(!(o instanceof Pair)) 
            return false;
        IntPair p = (IntPair)o;
        return p.getsource() == getsource() && p.gettarget() == gettarget();
    }
    
    private volatile int hashCode; // define this as a member of the class

@Override public int hashCode() {
    int result = hashCode;
    if(result == 0) {
        result = 17 * getsource() + 31 * gettarget();
        hashCode = result;
    }
    return result;
}
    //Unit Test for IntPair Structure
    public static void main(String []args){
        HashMap<IntPair,Integer> edges;
        edges=new HashMap<IntPair,Integer>(10);
        IntPair o=new IntPair(305,306);
        IntPair o2=new IntPair(305,204);
        IntPair o3=new IntPair(305,306);
        edges.put(o, 1);
        if(edges.containsKey(o2)==false){
            edges.put(o2, 1);
        }else{
            int freq=edges.get(o2);
            edges.put(o2, freq+1);
        }
        if(edges.containsKey(o3)==false){
            edges.put(o3, 1);
        }else{
            int freq=edges.get(o3);
            edges.put(o3, freq+1);
        }
         Iterator it = edges.entrySet().iterator();
            while(it.hasNext()){
                Map.Entry pairs = (Map.Entry)it.next();
                IntPair p=(IntPair)pairs.getKey();
                System.out.print(p.getsource()+"\t"+p.gettarget()+"\t"+pairs.getValue()+"\n");
            }
    }
}
