/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package io.core;

import com.google.common.cache.Cache;
import com.google.common.cache.CacheBuilder;
import io.core.InterfaceJNI.TrackedPerson;
import io.core.PeopleManager.MyPerson;
import io.swagger.model.Body1;
import io.swagger.model.Body2;
import io.swagger.model.Body5;
import io.swagger.model.Body6;
import io.swagger.model.Body7;
import io.swagger.model.Body8;
//import io.swagger.model.Body9;
import io.swagger.model.Entity;
import io.swagger.model.IdentifiedPerson;
import io.swagger.model.Identity;
import io.swagger.model.Person;
import io.swagger.model.Position;
import io.swagger.model.Room;
import io.swagger.model.Stream;
import java.awt.image.BufferedImage;
import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.ByteArrayOutputStream;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.io.InputStream;
import java.io.OutputStream;
import java.io.OutputStreamWriter;
import java.io.PrintWriter;
import java.io.Writer;
import java.nio.file.FileSystems;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Objects;
import java.util.UUID;
import java.util.concurrent.TimeUnit;
import java.util.logging.Level;
import java.util.logging.Logger;
import javax.imageio.ImageIO;
import javax.ws.rs.Path;
import javax.ws.rs.WebApplicationException;
import javax.ws.rs.core.Response;
import javax.ws.rs.core.StreamingOutput;
import org.glassfish.jersey.media.multipart.BodyPartEntity;
import org.glassfish.jersey.media.multipart.FormDataBodyPart;
import org.glassfish.jersey.media.multipart.FormDataContentDisposition;
import org.glassfish.jersey.message.internal.Utils;

/**
 *
 * @author jack1
 */
public class MainSystem {
    static Map<Integer, MyRoom> rooms = new HashMap<Integer, MyRoom>();
    static Map<Integer, Stream> streams = new HashMap<Integer, Stream>();
    static Map<Integer, Entity> entities = new HashMap<Integer, Entity>();
    static Map<Integer, Identity> identities = new HashMap<Integer, Identity>();
    //static Map<Integer, MyPerson> mypeople = new HashMap<Integer, MyPerson>();
    static PeopleManager peopleManager = new PeopleManager();

    static int IDs=0;
    static int identityIDs=0;
    static int streamIDs=0;

    private static List<byte[]> imageByteList = new ArrayList<byte[]>(0);
    private static Map<Integer, Integer> currentIndex =new HashMap<Integer, Integer>();
    private static Cache<Integer, BufferedImage> images;

    
   
    private static Cache<Integer, BufferedImage> getImages(){
        if(images == null){
            images = CacheBuilder.newBuilder()
                        .expireAfterWrite(20, TimeUnit.SECONDS) 
                        .build();
        }
        return images;
    }
              

    //classi wrapper contenenti le info da inviare al client
    public static class ReturnRoomMessage{
        private String message;
        private Room payload;
        private StatusCode code;

        public ReturnRoomMessage(StatusCode code) {            
            this.code = code;
            if (code==StatusCode.OK){
                this.message = "successful operation";
            }
            if (code==StatusCode.NOT_FOUND){
                this.message = "Room ID not found";
            }
            if (code==StatusCode.INVALID){
                this.message = "Invalid ID supplied";
            }
        }

        public String getMessage() {
            return message;
        }

        public StatusCode getCode() {
            return code;
        }

        public ReturnRoomMessage setMessage(String message) {
            this.message = message;
            return this;
        }

        public void setCode(StatusCode code) {
            this.code = code;
        }

        public Room getPayload() {
            return payload;
        }

        public ReturnRoomMessage setPayload(Room payload) {
            this.payload = payload;
            return this;
        }
        
        
        
    }
    public static class ReturnRoomsMessage{
        private String message;
        private Map<Integer, Room> payload;
        private StatusCode code;

        public ReturnRoomsMessage(StatusCode code) {            
            this.code = code;
            if (code==StatusCode.OK){
                this.message = "successful operation";
            }            
        }

        public String getMessage() {
            return message;
        }

        public StatusCode getCode() {
            return code;
        }

        public ReturnRoomsMessage setMessage(String message) {
            this.message = message;
            return this;
        }

        public void setCode(StatusCode code) {
            this.code = code;
        }

        public Map<Integer, Room> getPayload() {
            return payload;
        }

        public ReturnRoomsMessage setPayload(Map<Integer, Room> payload) {
            this.payload = payload;
            return this;
        }

       
        
        
        
    }
    public static class ReturnPeopleMessage{
        private String message;
        private List<IdentifiedPerson> payload;
        private StatusCode code;

        public ReturnPeopleMessage(StatusCode code) {            
            this.code = code;
            if (code==StatusCode.OK){
                this.message = "successful operation";
            }
        }

        public String getMessage() {
            return message;
        }

        public StatusCode getCode() {
            return code;
        }

        public ReturnPeopleMessage setMessage(String message) {
            this.message = message;
            return this;
        }

        public void setCode(StatusCode code) {
            this.code = code;
        }

        public List<IdentifiedPerson> getPayload() {
            return payload;
        }

        public ReturnPeopleMessage setPayload(List<IdentifiedPerson> payload) {
            this.payload = payload;
            return this;
        }
        
        
        
    }
    public static class ReturnStreamsMessage{
        private String message;
        private List<Integer> payload;
        private StatusCode code;

        public ReturnStreamsMessage(StatusCode code) {            
            this.code = code;
            if (code==StatusCode.OK){
                this.message = "successful operation";
            }
            if (code==StatusCode.NOT_FOUND){
                this.message = "Stream ID not found";
            }
            if (code==StatusCode.INVALID){
                this.message = "Invalid ID supplied";
            }
        }


        public String getMessage() {
            return message;
        }

        public StatusCode getCode() {
            return code;
        }

        public ReturnStreamsMessage setMessage(String message) {
            this.message = message;
            return this;
        }

        public void setCode(StatusCode code) {
            this.code = code;
        }

        public List<Integer> getPayload() {
            return payload;
        }

        public ReturnStreamsMessage setPayload(List<Integer> payload) {
            this.payload = payload;
            return this;
        }
                
    }
    public static class ReturnStreamMessage{
        private String message;
        private Object payload; //TODO: gestire il flusso video
        private StatusCode code;

        public ReturnStreamMessage(StatusCode code) {            
            this.code = code;
            if (code==StatusCode.OK){
                this.message = "successful operation";
            }
            if (code==StatusCode.NOT_FOUND){
                this.message = "Stream ID not found";
            }
            if (code==StatusCode.INVALID){
                this.message = "Invalid ID supplied";
            }
        }


        public String getMessage() {
            return message;
        }

        public StatusCode getCode() {
            return code;
        }

        public ReturnStreamMessage setMessage(String message) {
            this.message = message;
            return this;
        }

        public void setCode(StatusCode code) {
            this.code = code;
        }

        public Object getPayload() {
            return payload;
        }

        public ReturnStreamMessage setPayload(Object payload) {
            this.payload = payload;
            return this;
        }
                
    }
    public static class ReturnPositionMessage{
        private String message;
        private Entity payload; //TODO: gestire il flusso video
        private StatusCode code;
        
        public ReturnPositionMessage(StatusCode code) {            
            this.code = code;
            if (code==StatusCode.OK){
                this.message = "successful operation";
            }
            if (code==StatusCode.NOT_FOUND){
                this.message = "Identity ID not found";
            }
            if (code==StatusCode.INVALID){
                this.message = "Invalid ID supplied";
            }
        }


        public String getMessage() {
            return message;
        }

        public StatusCode getCode() {
            return code;
        }

        public ReturnPositionMessage setMessage(String message) {
            this.message = message;
            return this;
        }

        public void setCode(StatusCode code) {
            this.code = code;
        }

        public Entity getPayload() {
            return payload;
        }

        public ReturnPositionMessage setPayload(Entity payload) {
            this.payload = payload;
            return this;
        }
                
    }
    public static class ReturnEntitiesMessage{
        private String message;
        private Map<Integer, Entity> payload;
        private StatusCode code;

        public ReturnEntitiesMessage(StatusCode code) {            
            this.code = code;
            if (code==StatusCode.OK){
                this.message = "successful operation";
            }            
        }

        public String getMessage() {
            return message;
        }

        public StatusCode getCode() {
            return code;
        }

        public ReturnEntitiesMessage setMessage(String message) {
            this.message = message;
            return this;
        }

        public void setCode(StatusCode code) {
            this.code = code;
        }

        public Map<Integer, Entity> getPayload() {
            return payload;
        }

        public ReturnEntitiesMessage setPayload(Map<Integer, Entity> payload) {
            this.payload = payload;
            return this;
        }

       
        
        
        
    }
    public static class ReturnIdentityMessage{
        private String message;
        private Identity payload;
        private StatusCode code;

        public ReturnIdentityMessage(StatusCode code) {            
            this.code = code;
            if (code==StatusCode.OK){
                this.message = "successful operation";
            }
            if (code==StatusCode.NOT_FOUND){
                this.message = "Identity ID not found";
            }
            if (code==StatusCode.INVALID){
                this.message = "Invalid ID supplied";
            }
        }

        public String getMessage() {
            return message;
        }

        public StatusCode getCode() {
            return code;
        }

        public ReturnIdentityMessage setMessage(String message) {
            this.message = message;
            return this;
        }

        public void setCode(StatusCode code) {
            this.code = code;
        }

        public Identity getPayload() {
            return payload;
        }

        public ReturnIdentityMessage setPayload(Identity payload) {
            this.payload = payload;
            return this;
        }
        
        
        
    }
    public static class ReturnIdentitiesMessage{
        private String message;
        private Map<Integer, Identity> payload;
        private StatusCode code;

        public ReturnIdentitiesMessage(StatusCode code) {            
            this.code = code;
            if (code==StatusCode.OK){
                this.message = "successful operation";
            }
        }

        public String getMessage() {
            return message;
        }

        public StatusCode getCode() {
            return code;
        }

        public ReturnIdentitiesMessage setMessage(String message) {
            this.message = message;
            return this;
        }

        public void setCode(StatusCode code) {
            this.code = code;
        }

        public Map<Integer, Identity> getPayload() {
            return payload;
        }

        public ReturnIdentitiesMessage setPayload(Map<Integer, Identity> payload) {
            this.payload = payload;
            return this;
        }
        
        
        
    }
    public static class ReturnPersonMessage{
        private String message;
        private Person payload;
        private StatusCode code;

        public ReturnPersonMessage(StatusCode code) {            
            this.code = code;
            if (code==StatusCode.OK){
                this.message = "successful operation";
            }
            if (code==StatusCode.NOT_FOUND){
                this.message = "Person ID not found";
            }
            if (code==StatusCode.INVALID){
                this.message = "Invalid ID supplied";
            }
        }

        public String getMessage() {
            return message;
        }

        public StatusCode getCode() {
            return code;
        }

        public ReturnPersonMessage setMessage(String message) {
            this.message = message;
            return this;
        }

        public void setCode(StatusCode code) {
            this.code = code;
        }

        public Person getPayload() {
            return payload;
        }

        public ReturnPersonMessage setPayload(Person payload) {
            this.payload = payload;
            return this;
        }
        
        
        
    }
    
    

    
    public enum StatusCode{OK, NOT_FOUND, INVALID}

    
    // utils
    static Room MyRoomToSwagger(MyRoom myRoom){
        Room room = new Room();
        room.name(myRoom.name);
        room.id(myRoom.id);
        return room;
    }
    static Map<Integer, Room> MyRoomsToSwagger(Map<Integer, MyRoom> myrooms){
        Map<Integer, Room> r = new HashMap<Integer, Room>();
        for(int i : rooms.keySet()){
            r.put(i, MyRoomToSwagger(rooms.get(i)));
        }
        
        return r;
    }
    
    //RESTapi functions
    public static synchronized ReturnRoomMessage getRoomInfo(int roomID){
        if(roomID<0) return new ReturnRoomMessage(StatusCode.INVALID);
        if (!rooms.containsKey(roomID)) {   
            return new ReturnRoomMessage(StatusCode.NOT_FOUND);
        }
        return new ReturnRoomMessage(StatusCode.OK).setPayload(MyRoomToSwagger(rooms.get(roomID)));
                    
    }

    public static synchronized ReturnRoomMessage addRoom(String roomName){
        MyRoom room = new MyRoom(IDs++, roomName);
        rooms.put(room.getId(),room);
        return new ReturnRoomMessage(StatusCode.OK).setMessage(room.toString());
    }

    public static synchronized ReturnRoomMessage deleteRoom(int roomId){
        if(rooms.containsKey(roomId)){
            rooms.remove(roomId);
                new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnRoomMessage(StatusCode.NOT_FOUND);
    }

    public static synchronized ReturnRoomMessage updateRoom(int roomId, String roomName){
        if(rooms.containsKey(roomId)){
            rooms.get(roomId).setName(roomName);
            return new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnRoomMessage(StatusCode.NOT_FOUND);
    }

    public static synchronized ReturnRoomsMessage getRooms() {
        return new ReturnRoomsMessage(StatusCode.OK).setPayload(MyRoomsToSwagger(rooms));
    }
    
    public static synchronized ReturnPeopleMessage getPeopleInRoom(int roomID) {
        /*Map<Integer,Person> ret = new HashMap<Integer, Person>();
        for(Integer key : people.keySet()){
            if(people.get(key).getPosition().getRoomID()==roomID){
                ret.put(key, people.get(key));
            }
        }
        */
        if(!rooms.containsKey(roomID)) return new ReturnPeopleMessage(StatusCode.NOT_FOUND).setMessage("Bad Room ID");
        TrackedPerson[] tp = rooms.get(roomID).interfaceJNI.getTrackedPeople();
        IdentifiedPerson[] people = new IdentifiedPerson[tp.length];
        int i=0;
        for(TrackedPerson elem : tp){
            people[i].setId(elem.ID);
            boolean b = !elem.name.equals("-");
            people[i].setIdentified(b);
            Position position = new Position();
            position.setX((float)elem.X);
            position.setY((float)elem.Y);
            position.setRoomID(roomID);
            people[i].setPosition(position); 
            people[i].setIdentified(b);
            Identity ident = new Identity();
            ident.firstname(elem.name);
            ident.setId(0);
            people[i].setIdentity(ident);
            i++;
        }
        peopleManager.updatePeopleInARoom(people);
        return new ReturnPeopleMessage(StatusCode.OK).setPayload(peopleManager.getPeopleInARoom(roomID));
        
    }
    
    public static synchronized ReturnStreamsMessage getStreams(Integer roomID) {
       if(roomID<0) return new ReturnStreamsMessage(StatusCode.INVALID);
       return new ReturnStreamsMessage(StatusCode.OK).setPayload(rooms.get(roomID).streams); 
    }
    
    public static Response getStream(Integer streamID){
        if(streamID<0) return Response.status(Response.Status.BAD_REQUEST).entity("wrong stream id").build();
        boolean isPresent = false;
        MyRoom room = null;
        int j = 0;
        String dir = "";
        for(int i=0; i<rooms.size();i++){
            if(rooms.get(i).streams.contains(streamID)) {
                isPresent = true;
                room = rooms.get(i);
            }
        }
        for(j=0;j<room.getStreams().size();j++)
            if(room.getStreams().get(j) == streamID){                
                if(j==0) dir = "";                     
                if(j==1) dir = "\\rawforeground";
                if(j==2) dir = "\\foreground";
                if(j==3) dir = "\\disparity";
                if(j==4) dir = "\\edge";
                if(j==5) dir = "\\occupancy";
                if(j==6) dir = "\\height";                
                if(j==7) dir = "\\background"; 

                break;
            }
        if(!isPresent) return Response.status(Response.Status.NOT_FOUND).build();
        int i=0;
        getImages();
        currentIndex.put(streamID, 0);
        //while(true){
            try {
                BufferedImage originalImage =null;
                int n=0;
                System.out.println("wait");
                do{                                
                    File mutex = new File("D:\\github\\plathea\\jaxrs-jersey-server-generated\\room"+room.id+dir+"\\mutex.txt");                    
                    BufferedReader br = new BufferedReader(new FileReader(mutex));
                    n = Integer.parseInt(br.readLine());
                }
                while( n<= currentIndex.get(streamID));  
                currentIndex.put(streamID, currentIndex.get(streamID)+1);
                                               
                    File file = new File("D:\\github\\plathea\\jaxrs-jersey-server-generated\\room"+room.id+dir+"\\frame0.jpeg");                     
                    //if(!image.exists()) break;
                    originalImage = ImageIO.read(file);
                if(originalImage==null) return Response.ok("no stream available").build();
                images.put(streamID, originalImage);
                
                //ByteArrayOutputStream baos = new ByteArrayOutputStream();
                //ImageIO.write( originalImage, "jpeg", baos );
                //baos.flush();
                //imageByteList.add(baos.toByteArray());
                i++;
                System.out.println("aggiungo image");
                //baos.close();                
            }catch (Exception ex) {
                System.err.println(ex);
                System.out.println("errore verificato al frame: "+i);
                return Response.ok().build();             
            }
        //}
        return streamVideo(streamID, room.id, dir);
    }
    
    // stream video
    public static Response streamVideo(final int streamID, final int roomID, final String dir) {
        StreamingOutput output  = new StreamingOutput() {
            private BufferedImage prevImage = null;       
            @Override
            public void write(OutputStream outputStream) throws IOException, WebApplicationException {
                BufferedImage image = null;
                System.out.println("invio nframe: "+images.size());
                try{
                    //int streamID = 0;                    
                    while((image = images.getIfPresent(streamID)) != null){
                        if(prevImage == null || !image.equals(prevImage)){
                            //System.out.println("data...");
                            //streamID++;
                            ByteArrayOutputStream baos = new ByteArrayOutputStream();
                            //System.out.println("trasformo in ByteArrayOutputStream");
                            ImageIO.write(image, "jpg", baos);
                            //System.out.println("trasformo in byte ");
                            byte[] imageData = baos.toByteArray();
                            //System.out.println("write1 ");
                            //System.out.println(imageData.length);
                            

                            outputStream.write((
                                            "--BoundaryString\r\n" +
                                            "Content-type: image/jpeg\r\n" +
                                            "Content-Length: "+imageData.length+"\r\n\r\n").getBytes());
                            //System.out.println("write2 ");
                            outputStream.write(imageData);
                            //System.out.println("write3 ");
                            outputStream.write("\r\n\r\n".getBytes());
                            outputStream.flush();
                            //System.out.println("leggo");
                            BufferedImage originalImage = null;
                            int n=0;
                            //System.out.println("wait");
                            do{                                
                                File mutex = new File("D:\\github\\plathea\\jaxrs-jersey-server-generated\\room"+roomID+dir+"\\mutex.txt");                    
                                BufferedReader br = new BufferedReader(new FileReader(mutex));
                                n = Integer.parseInt(br.readLine());
                            }
                            while( n<= currentIndex.get(streamID));  
                            currentIndex.put(streamID, currentIndex.get(streamID)+1);
                            do{                                
                                File file = new File("D:\\github\\plathea\\jaxrs-jersey-server-generated\\room"+roomID+dir+"\\frame"+currentIndex.get(streamID)+".jpeg");                     
                                //if(!image.exists()) break;
                                originalImage = ImageIO.read(file);
                            }
                            while(originalImage==null);     
                            
                            //System.out.println("put "+ images + "original image"+ originalImage);
                            images.put(streamID, originalImage);
                            //System.out.println("done ");
                        }
                        try {
                            TimeUnit.MILLISECONDS.sleep(100);
                            /*
                            try {
                            image.notifyAll();
                            image.wait();
                            } catch (InterruptedException e) {
                            // just read the next image
                            }
                            */
                        } catch (InterruptedException ex) {
                            System.out.println("errore sleep");
                            System.out.println(ex);
                        }
                    }
                    outputStream.flush();
                    outputStream.close();
                }catch(IOException ioe){
                        System.out.println("Steam closed by client!");
                }
          }
        };
        System.out.println("stream terminato");
        return Response.ok(output)
				.header("Connection", "close")
				.header("Max-Age", "0")
				.header("Expires", "0")
				.header("Cache-Control", "no-cache, private")
				.header("Pragma", "no-cache")
				.build();
    }
    
    public static synchronized ReturnPositionMessage getPositionFromEntityID(Integer entityID) {
        if(entityID<0) return new ReturnPositionMessage(StatusCode.INVALID);
        if (!entities.containsKey(entityID)) {   
            return new ReturnPositionMessage(StatusCode.NOT_FOUND);
        }
        else{
            return new ReturnPositionMessage(StatusCode.OK).setPayload(entities.get(entityID));
        } 
    }
    
    public static synchronized ReturnEntitiesMessage getAllPositions() {
        return new ReturnEntitiesMessage(StatusCode.OK).setPayload(entities);
    }

    public static synchronized ReturnIdentityMessage addIdentity(Body8 body){
        Identity identity = new Identity();
        identity.firstname(body.getFirstname());
        identity.setSurname(body.getSurname());
        identity.setId(identityIDs++);
        identities.put(identity.getId(),identity);
        return new ReturnIdentityMessage(StatusCode.OK);
    }

    public static synchronized ReturnIdentityMessage deleteIdentity(Integer identityID) {
        if(identities.containsKey(identityID)){
            identities.remove(identityID);
                new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnIdentityMessage(StatusCode.NOT_FOUND);    
    }
    
    public static synchronized ReturnIdentityMessage getIdentityInfo(Integer identityID) {
        if(identityID<0) return new ReturnIdentityMessage(StatusCode.INVALID);
        if (!identities.containsKey(identityID)) {   
            return new ReturnIdentityMessage(StatusCode.NOT_FOUND);
        }
        else{
            return new ReturnIdentityMessage(StatusCode.OK).setPayload(identities.get(identityID));
        }     
    }
    
    public static synchronized ReturnIdentityMessage updateIdentity(Integer identityID, Body7 body) {
        if(identities.containsKey(identityID)){
            identities.get(identityID).firstname(body.getFirstname());
            identities.get(identityID).surname(body.getSurname());
            return new ReturnIdentityMessage(StatusCode.OK);
        }
        return new ReturnIdentityMessage(StatusCode.NOT_FOUND);   
    }
    
    public static synchronized ReturnIdentitiesMessage getIdentities() {
        return new ReturnIdentitiesMessage(StatusCode.OK).setPayload(identities);
    }
    
    public static synchronized ReturnPersonMessage getPerson(Integer personID) {
        if(personID<0) return new ReturnPersonMessage(StatusCode.INVALID);
        Person p = peopleManager.getPerson(personID);
        if (p!=null) return new ReturnPersonMessage(StatusCode.OK).setPayload(p);               
        return new ReturnPersonMessage(StatusCode.NOT_FOUND);              
    }
    
    public static synchronized ReturnPersonMessage getPersonWithIdentity(Integer personID) {
        if(personID<0) return new ReturnPersonMessage(StatusCode.INVALID);
        Person p = peopleManager.getPersonWithIdentity(personID);
        if (p!=null) return new ReturnPersonMessage(StatusCode.OK).setPayload(p);      
        return new ReturnPersonMessage(StatusCode.OK).setPayload(p);
    }
    
    public static synchronized ReturnPeopleMessage getPeople() {        
        return new ReturnPeopleMessage(StatusCode.OK).setPayload(peopleManager.getPeople());
    }
    
    
    //PLATHEA
    public static synchronized ReturnRoomMessage facedatabase(Integer roomID, List<FormDataBodyPart> bodyParts, FormDataContentDisposition fileDispositions){ 
        if (bodyParts == null || bodyParts.isEmpty()) return new ReturnRoomMessage(StatusCode.INVALID).setMessage("No files in the body");
        if(rooms.containsKey(roomID)){
            new File("room"+roomID+"\\FaceDatabase").mkdirs();
            for(int i=0;i<bodyParts.size();i++){
                // save it
                StringBuffer fileDetails = new StringBuffer("");		
                /*
                 * Casting FormDataBodyPart to BodyPartEntity, which can give us
                 * InputStream for uploaded file
                 */
                BodyPartEntity bodyPartEntity = (BodyPartEntity) bodyParts.get(i).getEntity();
                String fileName = bodyParts.get(i).getContentDisposition().getFileName();
                String path = "D:\\github\\plathea\\jaxrs-jersey-server-generated\\room"+roomID+"\\FaceDatabase\\"+fileName;                 

                //writeToFile(bodyPartEntity.getInputStream(), path);			
		
            }
                       
            return new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnRoomMessage(StatusCode.NOT_FOUND); 
    }
    
    public static synchronized ReturnRoomMessage loadconfigurationfile(Integer roomID, InputStream uploadedInputStream, FormDataContentDisposition fileDetail) {   
        if (uploadedInputStream.equals(null)) return new ReturnRoomMessage(StatusCode.INVALID);
        if(rooms.containsKey(roomID)){
            String path = "D:\\github\\plathea\\jaxrs-jersey-server-generated\\room"+roomID;
            new File(path).mkdirs();
            new File(path+"\\background").mkdir();
            new File(path+"\\rawforeground").mkdir();
            new File(path+"\\foreground").mkdir();
            new File(path+"\\disparity").mkdir();
            new File(path+"\\edge").mkdir();
            new File(path+"\\occupancy").mkdir();
            new File(path+"\\height").mkdir();

            try {
                new File(path+"\\background\\mutex.txt").createNewFile();
                new File(path+"\\rawforeground\\mutex.txt").createNewFile();
                new File(path+"\\foreground\\mutex.txt").createNewFile();
                new File(path+"\\disparity\\mutex.txt").createNewFile();
                new File(path+"\\edge\\mutex.txt").createNewFile();
                new File(path+"\\occupancy\\mutex.txt").createNewFile();
                new File(path+"\\height\\mutex.txt").createNewFile();
            } catch (IOException ex) {
                Logger.getLogger(MainSystem.class.getName()).log(Level.SEVERE, null, ex);
            }
            
            
            String uploadedFileLocation = path+"\\" + fileDetail.getFileName();
            //writeToFile(uploadedInputStream, uploadedFileLocation);                      
            rooms.get(roomID).interfaceJNI.loadConfigurationFile(roomID, uploadedFileLocation);
            return new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnRoomMessage(StatusCode.NOT_FOUND); 
    }
    
    public static synchronized ReturnRoomMessage internalcalibration(Integer roomID,Integer mask, List<FormDataBodyPart> bodyParts, FormDataContentDisposition fileDispositions) { 
        if (bodyParts == null || bodyParts.isEmpty()) return new ReturnRoomMessage(StatusCode.INVALID).setMessage("No files in the body");
        String[] name = {"3DReprojection.xml", "Essential.xml", "Fundamental.xml", 
                "LeftDistortion.xml", "LeftIntrinsics.xml", "mx_LEFT.xml", "mx_RIGHT.xml",
                "my_LEFT.xml", "my_RIGHT.xml", "RightDistortion.xml", "RightIntrinsics.xml",
                "Rotation.xml", "Traslation.xml"};
        if(rooms.containsKey(roomID)){
            String path = "D:\\github\\plathea\\jaxrs-jersey-server-generated\\room"+roomID+"\\InternalCalibration";
            new File(path).mkdirs();
            for(int i=0;i<bodyParts.size();i++){
                // save it
                StringBuffer fileDetails = new StringBuffer("");		
                /*
                 * Casting FormDataBodyPart to BodyPartEntity, which can give us
                 * InputStream for uploaded file
                 */
                BodyPartEntity bodyPartEntity = (BodyPartEntity) bodyParts.get(i).getEntity();
                String fileName = bodyParts.get(i).getContentDisposition().getFileName();
                String uploadedFileLocation = path+"\\"+fileName;                 

                //writeToFile(bodyPartEntity.getInputStream(), uploadedFileLocation);			
		
            }
            rooms.get(roomID).interfaceJNI.internalCalibration(path, mask);         
            return new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnRoomMessage(StatusCode.NOT_FOUND); 
    }
    
    public static synchronized ReturnRoomMessage externalcalibration(Integer roomID, List<FormDataBodyPart> bodyParts, FormDataContentDisposition fileDispositions) {   
        if (bodyParts == null || bodyParts.isEmpty()) return new ReturnRoomMessage(StatusCode.INVALID).setMessage("No files in the body");
        String[] name = {"3DReprojection.xml", "Essential.xml", "Fundamental.xml", 
                "LeftDistortion.xml", "LeftIntrinsics.xml", "mx_LEFT.xml", "mx_RIGHT.xml",
                "my_LEFT.xml", "my_RIGHT.xml", "RightDistortion.xml", "RightIntrinsics.xml",
                "Rotation.xml", "Traslation.xml"};
        if(rooms.containsKey(roomID)){
            String path = "D:\\github\\plathea\\jaxrs-jersey-server-generated\\room"+roomID+"\\ExternalCalibration";
            new File(path).mkdirs();
            for(int i=0;i<bodyParts.size();i++){
                // save it
                StringBuffer fileDetails = new StringBuffer("");		
                /*
                 * Casting FormDataBodyPart to BodyPartEntity, which can give us
                 * InputStream for uploaded file
                 */
                BodyPartEntity bodyPartEntity = (BodyPartEntity) bodyParts.get(i).getEntity();
                String fileName = bodyParts.get(i).getContentDisposition().getFileName();
                String uploadedFileLocation = path+"\\"+fileName;                 

                //writeToFile(bodyPartEntity.getInputStream(), uploadedFileLocation);			
		
            }
            rooms.get(roomID).interfaceJNI.externalCalibration(path);         
            return new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnRoomMessage(StatusCode.NOT_FOUND); 
    }
    
    public static synchronized ReturnRoomMessage selectsvmclassifier(Integer roomID, InputStream uploadedInputStream, FormDataContentDisposition fileDetail) {   
        if (uploadedInputStream.equals(null)) return new ReturnRoomMessage(StatusCode.INVALID);
        if(rooms.containsKey(roomID)){
            String path = "D:\\github\\plathea\\jaxrs-jersey-server-generated\\room"+roomID+"\\Tracking";
            new File(path).mkdirs();
            String uploadedFileLocation = path+"\\" + fileDetail.getFileName();
            //writeToFile(uploadedInputStream, uploadedFileLocation);            
            rooms.get(roomID).interfaceJNI.selectSVMclassifier(uploadedFileLocation);
            return new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnRoomMessage(StatusCode.NOT_FOUND); 
    }
    
    public static synchronized ReturnRoomMessage initializesystem(Integer roomID, String username, String password, String type, String resolution, Integer fps, String cameraModel, String ipAddress1, Integer port1, String ipAddress2, Integer port2) {
        if(rooms.containsKey(roomID)){
            rooms.get(roomID).interfaceJNI.initializeSystem(username, password, type, resolution, fps, cameraModel, ipAddress1, port1, ipAddress2, port2);
            return new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnRoomMessage(StatusCode.NOT_FOUND); 
    }
    
    public static synchronized ReturnRoomMessage startlocalizationengine(Integer roomID, Boolean withoutTracking, Boolean saveTracksToFile) {
        if(rooms.containsKey(roomID)){
            String path = "";
            rooms.get(roomID).interfaceJNI.startLocalizationEngine(withoutTracking, saveTracksToFile, path);
            return new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnRoomMessage(StatusCode.NOT_FOUND); 
    }
    
     public static synchronized ReturnRoomMessage platheaplayer(Integer roomID) {
        if(rooms.containsKey(roomID)){
            rooms.get(roomID).interfaceJNI.platheaPlayer();
            return new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnRoomMessage(StatusCode.NOT_FOUND); 
    }
    
    public static ReturnRoomMessage platheaplayerstart(Integer roomID) {
        if(rooms.containsKey(roomID)){
            String path = "D:\\github\\plathea\\jaxrs-jersey-server-generated\\room0\\Tests\\21-12-2012 - 11-25-10-165";
            rooms.get(roomID).interfaceJNI.platheaPlayerStart(path);
            rooms.get(roomID).streams.add(streamIDs++);  //left
            rooms.get(roomID).streams.add(streamIDs++);  //background    
            rooms.get(roomID).streams.add(streamIDs++);  //disparity
            rooms.get(roomID).streams.add(streamIDs++);  //edge          
            rooms.get(roomID).streams.add(streamIDs++);  //foreground                        
            rooms.get(roomID).streams.add(streamIDs++);  //height          
            rooms.get(roomID).streams.add(streamIDs++);  //occupancy          
            rooms.get(roomID).streams.add(streamIDs++);  //rawforeground          
        

                       
            return new ReturnRoomMessage(StatusCode.OK);
        }
        return new ReturnRoomMessage(StatusCode.NOT_FOUND); 
    }
    
    public static Response continuousTracking(int roomID){
        System.out.println("continuousTracking");
        StreamingOutput output  = new StreamingOutput() {
            int i=0;
            @Override
            public void write(OutputStream os) throws IOException,
            WebApplicationException {
              Writer writer = new BufferedWriter(new OutputStreamWriter(os));
              writer.write(i++);
              writer.flush();  // <-- This is very important.  Do not forget.
            }
        };
        System.out.println("stream terminato");
        return Response.ok(output).build();       
    }
     
    // save uploaded file to new location
    private static void writeToFile(InputStream uploadedInputStream,String uploadedFileLocation) {

		try {
			OutputStream out = new FileOutputStream(new File(
					uploadedFileLocation));
			int read = 0;
			byte[] bytes = new byte[1024];

			out = new FileOutputStream(new File(uploadedFileLocation));
			while ((read = uploadedInputStream.read(bytes)) != -1) {
				out.write(bytes, 0, read);
			}
			out.flush();
			out.close();
		} catch (IOException e) {

			e.printStackTrace();
		}

	}
    
    
}
    

    

