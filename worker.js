onmessage = function(e) {
  var id = e.data[0];
  var msg = e.data[1];

  if( id === 0 ){
    loadModule( msg )
  }
  
  if( id === 1 ){
    do_decode( msg )
  }
}

var __decoder = null;

function onWasmModuleLoaded( decoder ){
  __decoder = decoder;
}


function loadModule(descriptor){

  descriptor['onModuleLoaded'] = onWasmModuleLoaded
  DracoModule(descriptor);

}


function do_decode( decodeRequest ){

  var gid       = decodeRequest.gid
  var rawBuffer = decodeRequest.data
  var attribs = decodeRequest.attribs
  var stride = decodeRequest.stride


  var buffer = new __decoder.DecoderBuffer();
  buffer.Init(new Int8Array(rawBuffer), rawBuffer.byteLength);
  var wrapper = new __decoder.WebIDLWrapper();

  var dracoGeometry = wrapper.DecodePointCloudFromBuffer(buffer);
  __decoder.destroy(buffer);


  var numPoints = dracoGeometry.num_points();
  var numAttributes = dracoGeometry.num_attributes();

  var datas = []
  for (var i = 0; i < numAttributes; i++) {
    var attrib = wrapper.GetAttribute( dracoGeometry, i )
    var attribData = new __decoder.DracoFloat32Array();
    wrapper.GetAttributeFloatForAllPoints(
        dracoGeometry, attrib, attribData );

    datas.push( attribData )
  }


  var bytebuffer = interlace( attribs, datas, numPoints, stride )

  // free streams
  while( datas.length > 0 )
    __decoder.destroy( datas.pop() )
  

  postMessage( [2, {
    gid:gid,
    buffer : bytebuffer
  }] )

}




function interlace( bufferAttribs, attribs, numverts, stride ){
  // console.log( numverts, bufferAttribs, attribs )

  if( bufferAttribs.length !== attribs.length ){
    throw new Error( 'attrib len mismatch' )
  }

  var num_attribs = bufferAttribs.length;


  var fPerVerts = stride/4;

  var data = new Float32Array( numverts * fPerVerts )

  var offset = 0;
  for (var ai = 0; ai < num_attribs; ai++) {

    var battrib = bufferAttribs[ai];
    var numcomps = battrib.size
    var sattrib = attribs[ai]
    
    for (var vi = 0; vi < numverts; vi++) {
      var j = offset + vi * fPerVerts;
      var l = vi * numcomps;

      for (var k = 0; k < numcomps; k++) {
        data[j+k] = sattrib.GetValue(l+k)
      }
      
    }

    offset += numcomps;

  }

  return data.buffer;
  

}

