void main()
{
    /* Game Logic
    
    NOTES:
    * Use the walls to square yourself
    * Things you can do with "tank" drive:
    *   Pivot about one wheel
    *   Zero point turn
    *   Square after every 90 deg turn if possible
    
    
    Option A
    *** Prerequisites:
        Unable to Detect
        Able to harvest from all
        
    *** Comments:
        Assuming no source detection sources work, try for Solar first, Hydro second, and Wind third.
        
    *** Game Logic
    * START facing North
    * Start game timer
    * Go straight into wall
    * 90 deg turn CW
    * Go straight into SOLAR source
    * Raise Solar Panels
    * Wait 5 secs
    *
    * If battery charging
    *   Wait until Minute 1 is up
    *   Backup (.5 sec)
    *   170 deg turn CW (pivot about R wheel)
    *   Straight into wall (square robot)
    *   90 deg turn (in place)
    *   Go straight into HYDRO source
    *   
    *   If battery charging
    *       Wait until Minute 2 is up
    *       Backup (.5 sec)
    *       180 deg turn CW (pivot about R wheel)
    *       Go Straight (.5 sec)            // Enough to pass bucket
    *       Turn 90 deg (pivot about L wheel) -> Square robot
    *       Straight until far wall (~ 2-3 secs)
    *       90 deg turn (in place)
    *       Straight into flag
    *       DISCHARGE!
    *    
    *   Else
    *       Backup (.5 sec)
    *       170 deg turn CW (pivot about R wheel)
    *       Straight into wall (square robot)
    *       
    *       * 
    *       * Balmer's peak dried up
    *       * 
    *       * 
    * Else
    *   Back up until wall
    *   90 deg turn CW
    *   
    *   * 
    *   * Balmer's peak dried up
    *   * 
    *   * 
    */
}