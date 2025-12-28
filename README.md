create or alter procedure getordersalecategorysales (

    poutletid smallint,
    premotelocationid smallint,
    pforb char(1),
    psalecategoryid smallint,
    pitemgroupid smallint,
    fromtime timestamp,
    totime timestamp)

returns (

    itemid integer,
    amountsold numeric(18,4),
    grosssold numeric(18,4),
    salestax numeric(18,4),
    discount numeric(18,4),
    averageunitprice numeric(18,4),
    nettsold numeric(18,4),
    itemabbrev varchar(25),
    whendeleted timestamp,
    itemorder smallint,
    forb char(1),
    itemgroupid smallint,
    itemgroupabbrev varchar(25),
    itemgrouporder smallint,
    outletid smallint,
    outletname varchar(60),
    salecategoryid integer,
    salecategory varchar(20),
    superitemgroupid smallint,
    superitemgroup varchar(30),
    superitemgroupabbrev varchar(4),
    superitemgrouporder smallint,
    hideforb smallint,
    remotelocationid smallint,
    remotelocationname varchar(60))

as

declare variable currentsalecategoryid smallint;

declare variable lastoutletid smallint;

declare variable lastremotelocationid smallint;

begin

  currentsalecategoryid = 0;

  lastoutletid = 0;

  lastremotelocationid = -1;
 
  for select itemid, amountsold, grosssold, salestax, discount,

    averageunitprice, nettsold, outletid, remotelocationid, salecategoryid

    from getorderitemsalecategorysales (:poutletid, :premotelocationid, :pforb, :psalecategoryid, :pitemgroupid, :fromtime, :totime)

    order by salecategoryid, itemid

    into :itemid, :amountsold, :grosssold, :salestax, :discount,

    :averageunitprice, :nettsold, :outletid, :remotelocationid, :salecategoryid

  do begin

    if (hideforb = 1) then begin

      forb = '';

    end
 
    select i.itemabbrev, i.whendeleted, i.itemorder,

      ig.forb, ig.itemgroupid, ig.itemgroupabbrev, ig.itemgrouporder,

      sig.superitemgroupid, sig.superitemgroup, sig.superitemgroupabbrev, sig.superitemgrouporder, sig.hideforb

      from item i

      inner join itemgrp ig on (i.itemgroupid = ig.itemgroupid)

      left join superitemgroup sig on (ig.superitemgroupid = sig.superitemgroupid)

      where (i.itemid = :itemid)

      into :itemabbrev, :whendeleted, :itemorder,

      :forb, :itemgroupid, :itemgroupabbrev, :itemgrouporder,

      :superitemgroupid, :superitemgroup, :superitemgroupabbrev, :superitemgrouporder, :hideforb;
 
    if (lastoutletid <> outletid) then begin

      select outletname

        from outlet

        where (outletid = :outletid)

        into :outletname;
 
      lastoutletid = outletid;

    end
 
    if (salecategoryid is null) then begin

      salecategory = null;

    end

    else begin

      if (salecategoryid <> currentsalecategoryid) then begin

        select salecategoryid, salecategory

          from salecategory

          where (salecategoryid = :salecategoryid)

          into :currentsalecategoryid, :salecategory;

      end

    end
 
    if ((lastremotelocationid <> remotelocationid) and (remotelocationid is not null)) then begin

      if (remotelocationid = 0) then begin

        select remotelocationname from

        remotelocation

        where localdb = 1

        into :remotelocationname;

      end

      else begin

      select remotelocationname

        from remotelocation

        where (remotelocationid = :remotelocationid)

        into :remotelocationname;

      end

      lastremotelocationid = remotelocationid;

    end

    suspend;

  end

end^
 
SET TERM ; ^
 
/* Following GRANT statements are generated automatically */
 
GRANT EXECUTE ON PROCEDURE GETITEMSALECATEGORYSALES TO PROCEDURE GETSALECATEGORYSALES;

GRANT SELECT ON ITEM TO PROCEDURE GETSALECATEGORYSALES;

GRANT SELECT ON ITEMGRP TO PROCEDURE GETSALECATEGORYSALES;

GRANT SELECT ON SUPERITEMGROUP TO PROCEDURE GETSALECATEGORYSALES;

GRANT SELECT ON OUTLET TO PROCEDURE GETSALECATEGORYSALES;

GRANT SELECT ON SALECATEGORY TO PROCEDURE GETSALECATEGORYSALES;

GRANT SELECT ON REMOTELOCATION TO PROCEDURE GETSALECATEGORYSALES;
 
/* Existing privileges on this procedure */
 
GRANT EXECUTE ON PROCEDURE GETSALECATEGORYSALES TO SYSDBA;

GRANT EXECUTE ON PROCEDURE GETSALECATEGORYSALES TO WBDBA;

GRANT EXECUTE ON PROCEDURE GETSALECATEGORYSALES TO ADMACCESS;

GRANT EXECUTE ON PROCEDURE GETSALECATEGORYSALES TO STDACCESS;
 
SET TERM ^ ;

===================================================================================================================================================================================

create or alter procedure getorderitemsalecategorysales (

    poutletid smallint,
    premotelocationid smallint,
    pforb char(1),
    psalecategoryid smallint,
    pitemgroupid smallint,
    fromtime timestamp,
    totime timestamp)

returns (

    itemid integer,
    outletid smallint,
    remotelocationid smallint,
    amountsold numeric(18,4),
    grosssold numeric(18,4),
    salestax numeric(18,4),
    discount numeric(18,4),
    salecategoryid smallint,
    averageunitprice numeric(18,4),
    nettsold numeric(18,4))

as
declare variable repalldepartments smallint;
declare variable repdepartmentid smallint;
declare variable orderdepartmentid smallint;
declare variable igforb char(1);
declare variable igitemgroupid smallint;
declare variable itemcleared smallint;
begin

  select max(repalldepartments), max(repdepartmentid)
    from sys
    into :repalldepartments, :repdepartmentid;

  for
    select salecategoryid, itemid, outletid, remotelocationid, orderdepartmentid,
           amountsold, grosssold, salestax, discount
    from getallorderitemsalecategorysales
      (:poutletid, :premotelocationid, :psalecategoryid, :fromtime, :totime)
    into :salecategoryid, :itemid, :outletid, :remotelocationid, :orderdepartmentid,
         :amountsold, :grosssold, :salestax, :discount
  do begin

    if (amountsold is null) then amountsold = 0;
    if (grosssold is null) then grosssold = 0;
    if (salestax is null) then salestax = 0;
    if (discount is null) then discount = 0;

    if (amountsold = 0) then
      averageunitprice = 0;
    else
      averageunitprice = grosssold / amountsold;

    nettsold = grosssold - discount;
    itemcleared = 1;

    if ((pforb <> '') or (pitemgroupid <> -1)) then begin
      select ig.forb, ig.itemgroupid
        from item i
        inner join itemgrp ig on i.itemgroupid = ig.itemgroupid
        where i.itemid = :itemid
        into :igforb, :igitemgroupid;

      if (((pforb <> '') and (igforb <> pforb)) or
          ((pitemgroupid <> -1) and (pitemgroupid <> igitemgroupid))) then
        itemcleared = 0;
    end

    if ((itemcleared = 1) and
        ((repalldepartments = 1) or
         ((repalldepartments = 0) and
          (((repdepartmentid is null) and (orderdepartmentid is null)) or
           ((repdepartmentid is not null) and (orderdepartmentid = repdepartmentid)))))) then
      suspend;

  end
end^


=========================================================================================================================================================================================================


create or alter procedure getallorderitemsalecategorysales (

    poutletid smallint,
    premotelocationid smallint,
    psalecategoryid smallint, -- STAFFID (ORDER STAFF)
    fromtime timestamp,
    totime timestamp)

returns (

    itemid integer,
    outletid smallint,
    remotelocationid smallint,
    orderdepartmentid smallint,
    amountsold numeric(18,4),
    grosssold numeric(18,4),
    salestax numeric(18,4),
    discount numeric(18,4),
    salecategoryid smallint)

as
begin

  if ((:poutletid = -2) and (:premotelocationid = -2)) then begin

    for
      select
        i.salecategoryid,
        il.itemid,
        o.departmentid,
        sum(il.qty),
        sum(il.ilamount),
        sum(il.salestax),
        sum(il.discountamount)
      from invoice i
        inner join invline il on i.invoiceid = il.invoiceid
        inner join ordln ol on il.orderlineid = ol.orderlineid
        inner join anord o on ol.orderid = o.orderid
        inner join wblogin wl on o.whoordered = wl.loginid
      where
        i.wheninvoiced >= :fromtime
        and i.wheninvoiced < :totime
        and ((:psalecategoryid = -1) or (wl.staffid = :psalecategoryid))
      group by
        i.salecategoryid,
        il.itemid,
        o.departmentid
      into
        :salecategoryid,
        :itemid,
        :orderdepartmentid,
        :amountsold,
        :grosssold,
        :salestax,
        :discount
    do suspend;

  end
  else if (:poutletid = -2) then begin

    for
      select
        i.salecategoryid,
        il.itemid,
        o.remotelocationid,
        o.departmentid,
        sum(il.qty),
        sum(il.ilamount),
        sum(il.salestax),
        sum(il.discountamount)
      from invoice i
        inner join invline il on i.invoiceid = il.invoiceid
        inner join ordln ol on il.orderlineid = ol.orderlineid
        inner join anord o on ol.orderid = o.orderid
        inner join wblogin wl on o.whoordered = wl.loginid
      where
        i.wheninvoiced >= :fromtime
        and i.wheninvoiced < :totime
        and ((:premotelocationid = -1) or
             (:premotelocationid = 0 and o.remotelocationid is null) or
             (o.remotelocationid = :premotelocationid))
        and ((:psalecategoryid = -1) or (wl.staffid = :psalecategoryid))
      group by
        i.salecategoryid,
        il.itemid,
        o.remotelocationid,
        o.departmentid
      into
        :salecategoryid,
        :itemid,
        :remotelocationid,
        :orderdepartmentid,
        :amountsold,
        :grosssold,
        :salestax,
        :discount
    do begin
      if (remotelocationid is null) then remotelocationid = 0;
      suspend;
    end

  end
  else begin

    for
      select
        i.salecategoryid,
        il.itemid,
        o.outletid,
        o.remotelocationid,
        o.departmentid,
        sum(il.qty),
        sum(il.ilamount),
        sum(il.salestax),
        sum(il.discountamount)
      from invoice i
        inner join invline il on i.invoiceid = il.invoiceid
        inner join ordln ol on il.orderlineid = ol.orderlineid
        inner join anord o on ol.orderid = o.orderid
        inner join wblogin wl on o.whoordered = wl.loginid
      where
        i.wheninvoiced >= :fromtime
        and i.wheninvoiced < :totime
        and ((:poutletid = -1) or (o.outletid = :poutletid))
        and ((:premotelocationid = -1) or
             (:premotelocationid = 0 and o.remotelocationid is null) or
             (o.remotelocationid = :premotelocationid))
        and ((:psalecategoryid = -1) or (wl.staffid = :psalecategoryid))
      group by
        i.salecategoryid,
        il.itemid,
        o.outletid,
        o.remotelocationid,
        o.departmentid
      into
        :salecategoryid,
        :itemid,
        :outletid,
        :remotelocationid,
        :orderdepartmentid,
        :amountsold,
        :grosssold,
        :salestax,
        :discount
    do begin
      if (remotelocationid is null) then remotelocationid = 0;
      suspend;
    end

  end
end^


==================================================================================================================================================================================================


procedure TformReports.ShowStaffSalesReport;
begin
  try
    formQRStaffSales := TformQRStaffSales.Create(Application);

    Title1 := sStaffTitle1;
    Title2 := '';
    Title3 := Format(
      sDateRange,
      [
        FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM', FFromTime),
        FormatDateTime('ddd dd mmm yyyy hh:mm AM/PM', FToTime)
      ]
    );

    if (not CheckDate) then
      Exit;

    with dm.qrStaffSales do
    begin
      Close;
      UnPrepare;
      SQL.Clear;

      {---------------------------------------------
        INVOICED vs ORDERED STAFF SALES (SAFE SWITCH)
      ----------------------------------------------}
      if radByOrderStaff.Checked then
        SQL.Add(
          'select * from getorderstaffsales ' +
          '(:poutletid, :premotelocationid, :pforb, :pstaffid, :pitemgroupid, :fromtime, :totime)'
        )
      else
        SQL.Add(
          'select * from getstaffsales ' +
          '(:poutletid, :premotelocationid, :pforb, :pstaffid, :pitemgroupid, :fromtime, :totime)'
        );

      {---------------------------------------------
        ORDER BY OPTIONS (UNCHANGED)
      ----------------------------------------------}
      if radGroup.Checked then
      begin
        SQL.Add('order by outletname, remotelocationname, staffid, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGroupItemOrder;
      end
      else if radUnits.Checked then
      begin
        SQL.Add('order by outletname, remotelocationname, staffid, amountsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sUnitSold;
      end
      else if radPrice.Checked then
      begin
        SQL.Add('order by outletname, remotelocationname, staffid, averageunitprice desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sAvgUPrice;
      end
      else if radGross.Checked then
      begin
        SQL.Add('order by outletname, remotelocationname, staffid, grosssold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sGrossSold;
      end
      else if radDiscount.Checked then
      begin
        SQL.Add('order by outletname, remotelocationname, staffid, discount desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sDiscount;
      end
      else if radNett.Checked then
      begin
        SQL.Add('order by outletname, remotelocationname, staffid, nettsold desc, superitemgrouporder, forb desc, itemgrouporder, itemgroupabbrev, itemorder');
        Title2 := ', ' + sNetSold;
      end;

      {---------------------------------------------
        ITEM GROUP / FOOD-BEVERAGE LOGIC (UNCHANGED)
      ----------------------------------------------}
      if radItemGroup.Checked then
      begin
        ParamByName('pforb').AsString := '';
        if DBGridItemGroups.SelectedRows.Count > 1 then
        begin
          ParamByName('pitemgroupid').AsString :=
            GetSelectedIDString(DBGridItemGroups, 'itemgroupid');
          Title2 := Title2 + ', ' + sSelectedItemGroups;
        end
        else
        begin
          ParamByName('pitemgroupid').AsInteger := Glbs.ItemGroupID;
          Title2 := Title2 + ', ' + Glbs.ItemGroup;
        end;
      end
      else
      begin
        ParamByName('pitemgroupid').AsInteger := -1;

        if radFAndB.Checked then
        begin
          ParamByName('pforb').AsString := '';
          Title2 := ', ' + sFoodBev + Title2;
        end
        else if radFOnly.Checked then
        begin
          ParamByName('pforb').AsString := 'F';
          Title2 := ', ' + sFood + Title2;
        end
        else
        begin
          ParamByName('pforb').AsString := 'B';
          Title2 := ', ' + sBev + Title2;
        end;
      end;

      {---------------------------------------------
        OUTLET LOGIC (UNCHANGED)
      ----------------------------------------------}
      if cmbOutlets.ItemIndex = 0 then
      begin
        if chkOutletBreakdown.Checked and chkOutletBreakdown.Enabled then
          ParamByName('poutletid').AsInteger := -1
        else
          ParamByName('poutletid').AsInteger := -2;

        Title2 := sAllOutlets + Title2;
      end
      else
      begin
        ParamByName('poutletid').AsInteger := Glbs.OutletID;
        Title2 := Glbs.OutletName + Title2;
      end;

      {---------------------------------------------
        REMOTE LOCATION LOGIC (UNCHANGED)
      ----------------------------------------------}
      if cmbRemoteLocations.ItemIndex = 0 then
      begin
        if chkRemoteLocationBreakdown.Checked and chkRemoteLocationBreakdown.Enabled then
          ParamByName('premotelocationid').AsInteger := -1
        else
          ParamByName('premotelocationid').AsInteger := -2;
      end
      else
        ParamByName('premotelocationid').AsInteger := Glbs.RemoteLocationID;

      {---------------------------------------------
        STAFF LOGIC (UNCHANGED)
      ----------------------------------------------}
      if radStaffAll.Checked then
      begin
        ParamByName('pstaffid').AsInteger := -1;
        Title1 := Title1 + ', ' + sAllStaffTitle2;
      end
      else
      begin
        if DBGridStaffMembers.SelectedRows.Count > 1 then
        begin
          ParamByName('pstaffid').AsString :=
            GetSelectedIDString(DBGridStaffMembers, 'staffid');
          Title1 := Title1 + ', ' + sSelectedStaff;
          formQRStaffSales.MultipleStaff := True;
        end
        else
        begin
          ParamByName('pstaffid').AsInteger := Glbs.StaffID;
          Title1 := Title1 + ', ' + Glbs.StaffName;
        end;
      end;

      {---------------------------------------------}
      ParamByName('fromtime').AsDateTime := FFromTime;
      ParamByName('totime').AsDateTime := FToTime;

      Open;
    end;

    {---------------------------------------------
      EXPORT
    ----------------------------------------------}
    if chkExportFile.Checked then
    begin
      SaveDialog.Title := sStaffDlgTitle;
      SaveDialog.FileName :=
        'StaffSales' + FormatDateTime('yyyymmddhhnn', Now) + '.csv';

      if SaveDialog.Execute then
      begin
        formQRStaffSales.WBCSV.FileName := SaveDialog.FileName;
        formQRStaffSales.WBCSV.OpenToWrite;
      end;
    end;

    formQRStaffSales.QRStaffSales.ReportTitle := sStaffRepTitle;
    formQRStaffSales.QRStaffSales.Preview;

  finally
    formQRStaffSales.Free;
  end;
end;
