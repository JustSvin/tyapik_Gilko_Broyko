program aboba;

function func(a: integer; b: integer): integer
begin
  if b = 2 then begin
    b := a * a;
  end
  else begin
    b := a;
  end;
end;

procedure proc(a: string) begin
  var i: integer;
  for i := 1 to 10 do begin
    writeln(a);
  end;
end;

begin
  // proc(1);

  // 1 + "asdfdasf";

  // while 1 + 2 do begin
  // end;

  var a: integer;
  var b: integer;
  a := 1;
  b := 2;
  var c := (1 + 2) * 3 / (4 - 5) + a - b;
  var arr: array[1..25] of integer;
  func(a + b, a);
  (1 + 2) * 3 / (4 - 5);
  1 - 2 + 3 - 5 + 6;

  while true do begin
  end;
end.