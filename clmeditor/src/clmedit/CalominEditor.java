package clmedit;


/* Curse of the Lost Miner, level editor*/
public class CalominEditor {
    
    /*The editor*/
    static EditorFrame eFrame;
    
    public static void main(String[] args)  {
        
        eFrame=new EditorFrame();
        eFrame.pack();
        eFrame.setVisible(true);
    }
    
    public static void quitEditor() {
        System.exit(0);
    }
    
    
    
}
