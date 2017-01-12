

package phonegraph;

/**
 *
 * @author linhong
 */

/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.HashSet;
import java.util.Iterator;
import java.util.Map;
import java.util.Scanner;
import java.util.logging.Level;
import java.util.logging.Logger;
import org.json.simple.JSONArray;
import org.json.simple.JSONObject;
import org.json.simple.parser.JSONParser;
import org.json.simple.parser.ParseException;
import util.IntPair;
import util.StrUtil;

/**
 *
 * @author linhong
 */
public class PhoneGraph {
    HashMap<String,Integer> phonemaps;
    HashMap<Integer,HashSet<String>> phone2urls;
    HashMap<IntPair,Integer> edges;
    int phonenumber=0;
    public PhoneGraph(){
    }
    public void AddtoMap(String phone, String uri){
        if(this.phonemaps.containsKey(phone)==false){
            this.phonemaps.put(phone, phonenumber);
            HashSet<String> urls=new HashSet<String>(10);
            urls.add(uri);
            phone2urls.put(phonenumber,urls);
            phonenumber++;
        }else{
            int id=this.phonemaps.get(phone);
            HashSet<String> urls=this.phone2urls.get(id);
            urls.add(uri);
            phone2urls.put(id, urls);
        }
    }
    public void runPhoneMap(String dirname){
        ArrayList<String> filenames = StrUtil.Initfolder(dirname);
        System.out.println(filenames.size());
        phonemaps=new HashMap<String,Integer>(100000);
        phone2urls=new HashMap<Integer,HashSet<String>>(100000);
         for (int i = 0; i < filenames.size(); i++) {
             CreatedphoneMapGT(filenames.get(i));
         }
    }
    public void CreatedphoneOnly(String filename){
        try {
            phonemaps=new HashMap<String,Integer>(400000);
            FileReader fileReader = new FileReader(filename);
            // Always wrap FileReader in BufferedReader.
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            String line;
            int count=0;
            bufferedReader.readLine();
            while((line = bufferedReader.readLine()) != null) {
                Scanner sc = new Scanner(line);
                sc.useDelimiter("\t");
                Integer id = sc.nextInt();
                String phone = sc.next();
                phonemaps.put(phone, id);
                count++;
//                if(count%100==0){
//                    System.out.println(count);
//                }
            }
            bufferedReader.close();
        } catch (IOException ex) {
            Logger.getLogger(PhoneGraph.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    public void CreatedphoneMapGT(String filename){
        try {
           
            FileReader fileReader = new FileReader(filename);
            // Always wrap FileReader in BufferedReader.
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            String line;
            int count=0;
            while((line = bufferedReader.readLine()) != null) {
                JSONObject  documentInfo= (JSONObject) new JSONParser().parse(line);
                JSONArray phones=(JSONArray)documentInfo.get("high_precision_phone");
                //JSONObject rawdata = (JSONObject)documentInfo.get("crawl_data");
                String uri = (String)documentInfo.get("cdr_id");
                if(phones != null){
                    for(int i = 0; i < phones.size(); i ++){
                        JSONObject o=(JSONObject)phones.get(i);
                        JSONArray a=(JSONArray)o.get("result");
                        for (int j = 0; j < a.size(); j++) {
                            JSONObject entity = (JSONObject)a.get(j);
                            String phone = (String)entity.get("value");
                            AddtoMap(phone,uri);
                        } 
                    }
                    
                }
                JSONArray emails=(JSONArray)documentInfo.get("high_precision_email");
                if (emails != null) {
                    for(int i = 0; i < emails.size(); i ++){
                        JSONObject o=(JSONObject)emails.get(i);
                        JSONArray a=(JSONArray)o.get("result");
                        for (int j = 0; j < a.size(); j++) {
                            JSONObject entity = (JSONObject)a.get(j);
                            String email = (String)entity.get("value");
                            AddtoMap(email,uri);
                        } 
                    }
                }
                count++;
//                if(count%100==0){
//                    System.out.println(count);
//                }
            }
            bufferedReader.close();
        } catch (IOException ex) {
            Logger.getLogger(PhoneGraph.class.getName()).log(Level.SEVERE, null, ex);
        } catch (ParseException ex) {
            Logger.getLogger(PhoneGraph.class.getName()).log(Level.SEVERE, null, ex);
        }
    }
    public void PrintResults(String ID){
        FileWriter fstream1 = null;
        try {
            fstream1 = new FileWriter("phone_map"+ID+".txt",false);
            BufferedWriter out=new BufferedWriter(fstream1);
            Iterator it = phonemaps.entrySet().iterator();
            out.write("PhoneID\tPhone\n");
            while (it.hasNext()) {
                Map.Entry pairs = (Map.Entry)it.next();
                out.write(pairs.getValue()+ "\t" + pairs.getKey()+"\n");
            }
            out.close();
            fstream1=new FileWriter("phone2url"+ID+".txt",false);
            out=new BufferedWriter(fstream1);
            it=phone2urls.entrySet().iterator();
            out.write("PhoneID\turi_lists\n");
            while(it.hasNext()){
                Map.Entry pairs = (Map.Entry)it.next();
                out.write(pairs.getKey().toString());
                HashSet<String> newset=(HashSet<String>) pairs.getValue();
                Iterator iterator = newset.iterator();
                while(iterator.hasNext()){
                    out.write("\t"+iterator.next());
                }
                out.write("\n");
                //phone2urls.remove((Integer)pairs.getKey());
            }
            out.close();
            fstream1.close();
            phone2urls.clear();
        } catch (IOException ex) {
            Logger.getLogger(PhoneGraph.class.getName()).log(Level.SEVERE, null, ex);
        } 
    }
    public void runGraph(String dirname, int k) {
        ArrayList<String> filenames = StrUtil.Initfolder(dirname);
        edges=new HashMap<IntPair,Integer>(100000);
        for (int i = 0; i < filenames.size(); i++) {
            CreateGraphGT(filenames.get(i), k);
        }
    }
    public void CreateGraphGT(String filename, int k){
        try {
            FileReader fileReader = new FileReader(filename);
            // Always wrap FileReader in BufferedReader.
            BufferedReader bufferedReader = new BufferedReader(fileReader);
            String line;
            int count=0;
            HashSet<Integer> nodes=new HashSet<Integer>(10000);
            while((line = bufferedReader.readLine()) != null) {
                //ignore the prefix for phone and uri
                JSONObject documentInfo= (JSONObject) new JSONParser().parse(line);
                JSONArray phones=(JSONArray)documentInfo.get("high_precision_phone");
                //JSONObject rawdata = (JSONObject)documentInfo.get("crawl_data");
                if(phones != null){
                    for(int i = 0; i < phones.size(); i ++){
                        JSONObject o=(JSONObject)phones.get(i);
                        JSONArray a=(JSONArray)o.get("result");
                        for (int j = 0; j < a.size(); j++) {
                            JSONObject entity = (JSONObject)a.get(j);
                            String phone = (String)entity.get("value");
                            if(phonemaps.containsKey(phone)==true){
                                int id=phonemaps.get(phone);
                                nodes.add(id);
                            }
                        } 
                    }
                    
                }
                JSONArray emails=(JSONArray)documentInfo.get("high_precision_email");
                if (emails != null) {
                     for(int i = 0; i < emails.size(); i ++){
                        JSONObject o=(JSONObject)emails.get(i);
                        JSONArray a=(JSONArray)o.get("result");
                        for (int j = 0; j < a.size(); j++) {
                            JSONObject entity = (JSONObject)a.get(j);
                            String email = (String)entity.get("value");
                            if(phonemaps.containsKey(email)==true){
                                int id=phonemaps.get(email);
                                nodes.add(id);
                            }
                        } 
                    }
                }
                if (nodes.size() < k) {
                    Integer[] arr = nodes.toArray(new Integer[nodes.size()]);
                    for(int i = 0;i < arr.length; i++){
                        for(int j=(i+1);j<arr.length;j++){
                            IntPair o=new IntPair(arr[i],arr[j]);
                            if(arr[i] > arr[j]){
                                o.setKey(arr[j]);
                                o.setValue(arr[i]);
                            } 
                            if(this.edges.containsKey(o)==false){
                                this.edges.put(o, 1);
                            }else{
                                int freq=this.edges.get(o);
                                this.edges.put(o, freq+1);
                            }
                        }
                    }
                }
                nodes.clear();
                count++;
                if(count%100==0){
                    System.out.println(count);
                }
            }
        } catch (IOException ex) {
            Logger.getLogger(PhoneGraph.class.getName()).log(Level.SEVERE, null, ex);
        } catch (ParseException ex) {
            Logger.getLogger(PhoneGraph.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public void WriteGraph(String ID){
        FileWriter fstream1 = null;
        try {
            fstream1 = new FileWriter("phone_graph"+ID+".txt",false);
            BufferedWriter out=new BufferedWriter(fstream1);
            Iterator it = edges.entrySet().iterator();
            while(it.hasNext()){
                Map.Entry pairs = (Map.Entry)it.next();
                IntPair o=(IntPair)pairs.getKey();
                out.write(o.getsource()+"\t"+o.gettarget()+"\t"+pairs.getValue()+"\n");
            }
            out.close();
        } catch (IOException ex) {
            Logger.getLogger(PhoneGraph.class.getName()).log(Level.SEVERE, null, ex);
        } 
    }
    public static void main(String []args) {
        if (args.length <1) {
            System.out.println("Usage: [raw_json_file] [output_prefix_string] [k]");
            System.exit(3);
        }
        PhoneGraph  mygraph = new PhoneGraph();
        mygraph.runPhoneMap(args[0]);
        mygraph.PrintResults(args[1]);
        //mygraph.CreatedphoneOnly(args[1]);
        System.out.println("finish reading results");
        int k = Integer.parseInt(args[2]);
        mygraph.runGraph(args[0],k);
        mygraph.WriteGraph(args[1]);
    }
}
